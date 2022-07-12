#include "thirdthread.h"

ThirdThread::ThirdThread(QObject *parent)
    : QThread(parent)
{
    mThr = new ThrData;
    mBuf = (uchar *)malloc(RTU_BUF_SIZE); //申请内存  -- 随便用
    mSerial = new Serial_Trans(this); //串口线程
    mShm = get_share_mem(); // 获取共享内存
    isOpen = false;
    mTcpServer = new TcpServer(this);
    mTcpServer->init(20086,true);
}

bool ThirdThread::init(const QString &name1,const QString &name2)
{
    timer = new QTimer(this);
    timer->start(5000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    isOpen = mSerial->openSerial(name1); // 打开串口
    serialName1 = name1;
    if(isOpen){
        QTimer::singleShot(3*1000,this,SLOT(start()));  // 启动线程
    }else{
        serialName2 = name2;
        isOpen = mSerial->openSerial(name2); // 打开串口
        if(isOpen){
            QTimer::singleShot(3*1000,this,SLOT(start()));  // 启动线程
        }
    }
    return isOpen;
}

void ThirdThread::timeoutDone()
{
    if(!isOpen)
    {
        isOpen = mSerial->openSerial(serialName1); // 打开串口
        if(isOpen)
        {
            QTimer::singleShot(3*1000,this,SLOT(start()));  // 启动线程
        }else{
            isOpen = mSerial->openSerial(serialName2); // 打开串口
            if(isOpen){
                QTimer::singleShot(3*1000,this,SLOT(start()));  // 启动线程
            }
        }
    }
}

void ThirdThread::run()
{
    isRun = true;
    while(isRun)
    {
        transData();
        msleep(20);
        if(256==system(QString("ls /dev/usb/tty1-1.3").toLatin1().data())&&256==system(QString("ls /dev/usb/tty1-1.2").toLatin1().data()))
        {
            sleep(2);
            emit mSerial->closeSerialSig();
            isOpen = false;
            isRun = false;
            return;
        }
    }
}

void ThirdThread::setCrc(uchar *buf, int len)
{
    int rtn = len-2;
    ushort crc =  rtu_crc(buf, rtn);
    buf[rtn++] = 0xff & crc; /*低8位*/
    buf[rtn++] = crc >> 8;
}

void ThirdThread::transData()
{
    uchar *buf = mBuf;
    int rtn = mSerial->recvData(buf, 5); //接收数据-
    if(rtn > 2 ) {
        if(!validateData(rtn)) return; //解析并验证数据
        uchar id = mThr->addr / 0x20;
        uchar addr = mThr->addr % 0x20;
        if(id >=BUS_NUM || addr >= BOX_NUM) return;
//        sBoxData *box = &(mShm->data[id].box[addr]); //共享内存
        if(addr-1 < 0) return;//上海创建
        sBoxData *box = &(mShm->data[id].box[addr-1]);
        if(box->offLine < 1) return;

        if(mThr->fn == Fn_Get){ //获取数据 _ [未加长度位0时该回复数据]
            if(box->rtuLen > 0) {
                  box->rtuArray[0] = mThr->addr;//
                  setCrc(box->rtuArray, box->rtuLen);//
                  mSerial->sendData(box->rtuArray, box->rtuLen);

//                int rtn = 0;//上海创建
//                if(addr == 1)
//                    rtn = rtu_sent_to_input_packet(box);
//                else if(addr > 1)
//                    rtn = rtu_sent_to_output_packet(box);
//                if(mThr->data*2 == rtn)
//                    mSerial->sendData(box->rtuArray, rtn+5);

            } else {
                mSerial->sendData(buf, rtn);
            }
        } else if(mThr->fn == Fn_Set){ //发送数据
            mSerial->sendData(buf, rtn); //先回应同样的数据
            if(rtu[id] != NULL) {
                buf[0] = addr;
                setCrc(buf, rtn);
                rtu[id]->sendData(buf, rtn, 200); //[最好放入其他线程——暂时放这]
            }
        }else{ //功能码不合法

        }

    }else{

    }
}

bool ThirdThread::validateData(int rtn)
{
    uchar *buf = mBuf;
    buf = mBuf;
    mThr->addr = *(buf++);
    mThr->fn   = *buf++;
    ushort ll, hh;
    hh = *buf++;
    ll = *buf++;
    mThr->position = (hh<<8) + ll;
    hh = *buf++;
    ll = *buf++;
    mThr->data = (hh<<8) + ll;
    ll = *buf++;
    hh = *buf++;
    mThr->crc = (ll<<8) + hh;
    buf = mBuf;
    ushort hlcrc = rtu_crc(buf, rtn-2);
    ushort lhcrc = (hlcrc >> 8) + (hlcrc<<8);
    if(mThr->crc != lhcrc ) return false;
    return true;
}

uint ThirdThread::calcZeroCur(sBoxData *box)
{
    ushort v1= box->data.cur.value[0];
    ushort v2= box->data.cur.value[1];
    ushort v3= box->data.cur.value[2];
    if(v1==0 && v2==0 && v2==0) return 0;
    uint a = v1*v1;
    uint b = v2*v2;
    uint c = v3*v3;
    uint d = v1*v2;
    uint e = v2*v3;
    uint f = v1*v3;
    if(a+b+c-d-e-f<0) return 0;
    uint zeroLine = sqrt(a+b+c-d-e-f);
    return zeroLine;
}

/**
  * 功　能：发送始端箱数据打包
  * 入口参数：pkt -> 发送结构体
  * 出口参数：ptr -> 缓冲区
  * 返回值：打包后的长度
  */
uchar ThirdThread::rtu_sent_to_input_packet(sBoxData *box)
{
    uchar *ptr = box->rtuArray;
    memset(box->rtuArray,0,sizeof(box->rtuLen));
    *(ptr++) = mThr->addr;  /*地址码*/
    *(ptr++) = Fn_Get; /*功能码*/
    *(ptr++) = 0x28; /*功能码*///3

    /*填入输入开关*/
    for(int i = 0 ; i < 3 ; i++)
    {
        *(ptr++) = 0x00; /*高8位*/
        *(ptr++) = (0xff)&(box->data.sw[i]); /*低8位*/
    }//6

    /*填入输入电流*/
    for(int i = 0 ; i < 3 ; i++)
    {
        *(ptr++) = ((box->data.cur.value[i]) >> 8); /*高8位*/
        *(ptr++) = (0xff)&(box->data.cur.value[i]); /*低8位*/
    }//6//12

    /*填入输入零线电流*/
    ushort zeroLine =(ushort) calcZeroCur(box);
    *(ptr++) = ((zeroLine) >> 8); /*高8位*/
    *(ptr++) = (0xff)&(zeroLine); /*低8位*///2//14

    /*填入输入电压*/
    for(int i = 0 ; i < 3 ; i++)
    {
        *(ptr++) = ((box->data.vol.value[i]) >> 8); /*高8位*/
        *(ptr++) = (0xff)&(box->data.vol.value[i]); /*低8位*/
    }//6//20

    /*填入输入有功功率*/
    for(int i = 0 ; i < 3 ; i++)
    {
        *(ptr++) = ((box->data.pow.value[i]) >> 8); /*高8位*/
        *(ptr++) = (0xff)&(box->data.pow.value[i]); /*低8位*/
    }//6//26

    /*填入输入视在功率*/
    for(int i = 0 ; i < 3 ; i++)
    {
        *(ptr++) = ((box->data.apPow[i]) >> 8); /*高8位*/
        *(ptr++) = (0xff)&(box->data.apPow[i]); /*低8位*/
    }//32
    /*填入输入功率因数*/
    for(int i = 0 ; i < 3 ; i++)
    {
        *(ptr++) = ((box->data.pf[i]) >> 8); /*高8位*/
        *(ptr++) = (0xff)&(box->data.pf[i]); /*低8位*/
    }//38
    /*填入输入频率*/
    *(ptr++) = ((box->rate) >> 8); /*高8位*/
    *(ptr++) = (0xff)&(box->rate); /*低8位*///40

    /*填入CRC*/
    ushort crc =  rtu_crc(box->rtuArray, 40+3);
    *(ptr++) = 0xff & crc; /*低8位*/
    *(ptr++) = crc >> 8;
    return 40;
}

/**
  * 功　能：发送插接箱数据打包
  * 入口参数：pkt -> 发送结构体
  * 出口参数：ptr -> 缓冲区
  * 返回值：打包后的长度
  */
uchar ThirdThread::rtu_sent_to_output_packet(sBoxData *box)
{
    uchar *ptr = box->rtuArray;
    memset(box->rtuArray,0,sizeof(box->rtuLen));
    *(ptr++) = mThr->addr;  /*地址码*/
    *(ptr++) = Fn_Get; /*功能码*/
    *(ptr++) = 0x1e; /*功能码*///3

    /*填入输出电流*/
    for(int i = 0 ; i < 3 ; i++)
    {
        if(box->loopNum==3){
            *(ptr++) = ((box->data.cur.value[i]) >> 8); /*高8位*/
            *(ptr++) = (0xff)&(box->data.cur.value[i]); /*低8位*/
        }else{
            *(ptr++) = ((box->lineTgBox.cur[i]) >> 8); /*高8位*/
            *(ptr++) = (0xff)&(box->lineTgBox.cur[i]); /*低8位*/
        }
    }//6

    /*填入输出电压*/
    for(int i = 0 ; i < 3 ; i++)
    {
        if(box->loopNum==3){
            *(ptr++) = 0x00; /*高8位*/
            *(ptr++) = (0xff)&(box->data.sw[i]); /*低8位*/
        }else{
            *(ptr++) = 0x00;
            *(ptr++) = (0xff)&(box->data.sw[i*3]); /*低8位*/
        }
    }//6//12

    /*填入输出功率*/
    for(int i = 0 ; i < 3 ; i++)
    {
        if(box->loopNum==3){
            *(ptr++) = ((box->data.pow.value[i]) >> 8); /*高8位*/
            *(ptr++) = (0xff)&(box->data.pow.value[i]); /*低8位*/
        }else{
            *(ptr++) = ((box->lineTgBox.pow[i]) >> 8); /*高8位*/
            *(ptr++) = (0xff)&(box->lineTgBox.pow[i]); /*低8位*/
        }
    }//6//18


    /*填入输出电能*/
    for(int i = 0 ; i < 3 ; i++)
    {
        if(box->loopNum==3){
            *(ptr++) = ((box->data.ele[i]) >> 24); /*8位*/
            *(ptr++) = (0xff)&((box->data.ele[i]) >> 16); /*8位*/
            *(ptr++) = ((0xff)&(box->data.ele[i])>>8); /*8位*/
            *(ptr++) = (0xff)&(box->data.ele[i]); /*8位*/
        }else{
            *(ptr++) = ((box->lineTgBox.ele[i]) >> 24); /*8位*/
            *(ptr++) = (0xff)&((box->lineTgBox.ele[i]) >> 16); /*8位*/
            *(ptr++) = ((0xff)&(box->lineTgBox.ele[i])>>8); /*8位*/
            *(ptr++) = (0xff)&(box->lineTgBox.ele[i]); /*8位*/
        }
    }//6//30

    /*填入CRC*/
    ushort crc =  rtu_crc(box->rtuArray, 30+3);
    *(ptr++) = 0xff & crc; /*低8位*/
    *(ptr++) = crc >> 8;
    return 30;
}
