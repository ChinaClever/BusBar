#include "thirdthread.h"

ThirdThread::ThirdThread(QObject *parent)
    : QThread(parent)
{
    mThr = new ThrData;
    mBuf = (uchar *)malloc(RTU_BUF_SIZE); //申请内存  -- 随便用
    mSerial = new Serial_Trans(this); //串口线程
    mShm = get_share_mem(); // 获取共享内存
    isOpen = false;
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
        msleep(1);
        if(256==system(QString("ls /dev/usb/tty1-1.3").toLatin1().data())&&256==system(QString("ls /dev/usb/tty1-1.2").toLatin1().data()))
        {
            sleep(1);
            mSerial->closeSerial();
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

void ThirdThread::setting()
{

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
        sBoxData *box = &(mShm->data[id].box[addr]); //共享内存
//        if(box->offLine < 1) return;

        if(mThr->fn == Fn_Get){ //获取数据 _ [未加长度位0时该回复数据]
            if(box->rtuLen > 0) {
                box->rtuArray[0] = mThr->addr;
                setCrc(box->rtuArray, box->rtuLen);
                mSerial->sendData(box->rtuArray, box->rtuLen);
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
    mThr->position = hh<<8 + ll;
    hh = *buf++;
    ll = *buf++;
    mThr->data = hh<<8 + ll;
    ll = *buf++;
    hh = *buf++;
    mThr->crc = (ll<<8) + hh;
    buf = mBuf;
    ushort hlcrc = rtu_crc(buf, rtn-2);
    ushort lhcrc = (hlcrc >> 8) + (hlcrc<<8);
    if(mThr->crc != lhcrc ) return false;
    return true;
}
