#include "thirdthread.h"

ThirdThread::ThirdThread(QObject *parent)
    : QThread(parent)
{
    mThr = new ThrData;
    mBuf = (uchar *)malloc(RTU_BUF_SIZE); //申请内存  -- 随便用
    mSerial = new Serial_Trans(this); //串口线程
    mShm = get_share_mem(); // 获取共享内存
//    isOpen = false;
//    mTcpServer = new TcpServer(this);
//    mTcpServer->init(20086,true);
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
        if(!QFile::exists("/dev/usb/tty1-1.3")&&!QFile::exists("/dev/usb/tty1-1.2"))
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
                 memset(box->rtuArray , 0 , SRC_DATA_LEN_MAX);
//                  box->rtuArray[0] = mThr->addr;//
//                  setCrc(box->rtuArray, box->rtuLen);//
//                  mSerial->sendData(box->rtuArray, box->rtuLen);

                int rtn = 0;//北京瑞云智信科技部
                rtn = rtu_sent_to_input_packet(box);
                if(mThr->data*2 == rtn)
                    mSerial->sendData(box->rtuArray, rtn+5);

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
    //int i = 0;
    uint t = 0;
    ushort ut = 0;
    *(ptr++) = mThr->addr;  /*地址码*/
    *(ptr++) = Fn_Get; /*功能码*/
    *(ptr++) = 0x8e; /*功能码*///3

    *(ptr++) = 0x00;//i++;
    *(ptr++) = box->loopNum;//i++;
    *(ptr++) = 0x00;//i++;
    *(ptr++) = box->proNum;//i++;

    for(int k = 0 ; k < 3 ; k++){
        ut = box->env.tem.value[k];
        *(ptr++) = ut >> 8;//i++;
        *(ptr++) = (0xff)&ut;//i++;
        ut = box->env.tem.max[k];
        *(ptr++) = ut >> 8;//i++;
        *(ptr++) = (0xff)&ut;//i++;
        ut = box->env.tem.min[k];
        *(ptr++) = ut >> 8;//i++;
        *(ptr++) = (0xff)&ut;//i++;
    }
    ut = box->rate;
    *(ptr++) = ut >> 8;//i++;
    *(ptr++) = (0xff)&ut;//i++;
    ut = box->minRate;
    *(ptr++) = ut >> 8;//i++;
    *(ptr++) = (0xff)&ut;//i++;
    ut = box->maxRate;
    *(ptr++) = ut >> 8;//i++;
    *(ptr++) = (0xff)&ut;//i++;

    /*填入输入开关*/
    for(int k = 0 ; k < 3 ; k++)
    {
        ut = box->data.vol.value[k];
        *(ptr++) = ut >> 8;//i++;
        *(ptr++) = (0xff)&ut;//i++;
        ut = box->data.vol.max[k];
        *(ptr++) = ut >> 8;//i++;
        *(ptr++) = (0xff)&ut;//i++;
        ut = box->data.vol.min[k];
        *(ptr++) = ut >> 8;//i++;
        *(ptr++) = (0xff)&ut;//i++;
        ut = box->data.cur.value[k];
        *(ptr++) = ut >> 8;//i++;
        *(ptr++) = (0xff)&ut;//i++;
        ut = box->data.cur.max[k];
        *(ptr++) = ut >> 8;//i++;
        *(ptr++) = (0xff)&ut;//i++;
        ut = box->data.cur.min[k];
        *(ptr++) = ut >> 8;//i++;
        *(ptr++) = (0xff)&ut;//i++;

        t = box->data.apPow[k];
        *(ptr++) = (t >> 24);//i++; /*HH8位*/
        *(ptr++) = (0xff)&(t >> 16);//i++; /*HL8位*/
        *(ptr++) = (0xff)&(t >> 8);//i++; /*LH8位*/
        *(ptr++) = (0xff)&(t);//i++; /*LL8位*/
        t = box->data.pow.value[k];
        *(ptr++) = (t >> 24);//i++; /*HH8位*/
        *(ptr++) = (0xff)&(t >> 16);//i++; /*HL8位*/
        *(ptr++) = (0xff)&(t >> 8);//i++; /*LH8位*/
        *(ptr++) = (0xff)&(t);//i++; /*LL8位*/
        t = box->data.pow.max[k];
        *(ptr++) = (t >> 24);//i++; /*HH8位*/
        *(ptr++) = (0xff)&(t >> 16);//i++; /*HL8位*/
        *(ptr++) = (0xff)&(t >> 8);//i++; /*LH8位*/
        *(ptr++) = (0xff)&(t);//i++; /*LL8位*/
        t = box->data.pow.min[k];
        *(ptr++) = (t >> 24);//i++; /*HH8位*/
        *(ptr++) = (0xff)&(t >> 16);//i++; /*HL8位*/
        *(ptr++) = (0xff)&(t >> 8);//i++; /*LH8位*/
        *(ptr++) = (0xff)&(t);//i++; /*LL8位*/

        ut = box->data.pf[k];
        *(ptr++) = ut >> 8;//i++;
        *(ptr++) = (0xff)&ut;//i++;
        t = box->data.ele[k];
        *(ptr++) = (t >> 24);//i++; /*HH8位*/
        *(ptr++) = (0xff)&(t >> 16);//i++; /*HL8位*/
        *(ptr++) = (0xff)&(t >> 8);//i++; /*LH8位*/
        *(ptr++) = (0xff)&(t);//i++; /*LL8位*/
        *(ptr++) = 0x00;//i++; /*高8位*/
        *(ptr++) = (0xff)&(box->data.sw[k]);//i++; /*低8位*/
    }//6
    for(int k = 0 ; k < 3 ; k++){
        ut = box->data.pl[k];
        *(ptr++) = ut >> 8;//i++;
        *(ptr++) = (0xff)&ut;//i++;
    }

    /*填入CRC*/
//    qDebug()<<" i " << i << endl;
    ushort crc =  rtu_crc(box->rtuArray, 142+3);
    *(ptr++) = 0xff & crc; /*低8位*/
    *(ptr++) = crc >> 8;
    return 142;
}
