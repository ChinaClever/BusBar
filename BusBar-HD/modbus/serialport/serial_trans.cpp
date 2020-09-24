/**
  * 串口基本操作 打开、关闭、读写
  *         Lzy       2016-6-1
  */
#include "serial_trans.h"
#include "rtu485/rtu_sent.h"
#include "common.h"
Serial_Trans::Serial_Trans(QObject *parent) :
    QThread(parent)
{
    fd = -1;
}

Serial_Trans:: ~Serial_Trans()
{
    closeSerial();
}

/**
  * 功　能：打开端口
  * 入口参数：serialName> 设备名
  * 出口参数：
  * 返回值：true
  */
bool Serial_Trans::openSerial(const QString serialName)
{
    Serial_PortSet serialPort;
    Serial_PortData setData;
    serialPort.setPortName(serialName,setData); // 设置串口名

    struct termios setting;
    fd = open(qPrintable("/dev/" + setData.devName), O_RDWR);
    if (fd == -1) { //串口是否存在
        //QMessageBox::warning(this, "open error", strerror(errno));
        qDebug() << "Serial Err::open "<< serialName;
        goto openerr;
    }
    tcgetattr(fd, &setting);

    //  设置波特率
    speed_t speed;
    if (setData.baudRate == "B4800") {
        speed = B4800;
    } else if (setData.baudRate == "B9600") {
        speed = B9600;
    } else if (setData.baudRate == "B19200") {
        speed = B19200;
    } else if (setData.baudRate == "B38400") {
        speed = B38400;
    } else if (setData.baudRate == "B57600") {
        speed = B57600;
    } else if (setData.baudRate == "B115200") {
        speed = B115200;
    } else if (setData.baudRate == "B230400") {
        speed = B230400;
    } else if (setData.baudRate == "B460800") {
        speed = B460800;
    } else {
        //QMessageBox::warning(this, "error", "speed error!");
        goto seterr;
    }
    cfsetispeed(&setting, speed);
    cfsetospeed(&setting, speed);
    cfmakeraw(&setting);

    //  设置数据位
    if (setData.dataBit == "8") {
        setting .c_cflag |= CS8;
    } else if (setData.dataBit == "7") {
        setting.c_cflag |= CS7;
    } else if (setData.dataBit == "6") {
        setting.c_cflag |= CS6;
    } else if (setData.dataBit == "5") {
        setting.c_cflag |= CS5;
    } else {
        //QMessageBox::warning(this, "error", "dataBit error!");
        goto seterr;
    }

    //  设置parity
    if (setData.parity == "none") {
        setting.c_cflag &= ~PARENB;
        setting.c_iflag &= ~INPCK;
    } else if (setData.parity == "odd") {
        setting.c_cflag |= (PARODD | PARENB);
        setting.c_iflag |= INPCK;
    } else if (setData.parity == "even") {
        setting.c_cflag |= PARENB;
        setting.c_cflag &= ~PARODD;
        setting.c_iflag |= INPCK;
    } else {
        // QMessageBox::warning(this, "error", "dataBit error!");
        goto seterr;
    }

    //  设置停止位
    if (setData.stopBit == "1") {
        setting.c_cflag &= ~CSTOPB;
    } else if (setData.stopBit == "2") {
        setting.c_cflag |= CSTOPB;
    } else {
        //QMessageBox::warning(this, "error", "stopBit error!");
        goto seterr;
    }

    tcflush(fd, TCIFLUSH);
    setting.c_cc[VTIME] = 2; // 超时时间 0.5S
    setting.c_cc[VMIN] = 0; // 数据最小长度
    tcsetattr(fd, TCSANOW, &setting);

    //    monitor = new QSocketNotifier(fd, QSocketNotifier::Read, NULL);
    //    connect(monitor, SIGNAL(activated(int)), this, SLOT(readDataSlot()));
    //    monitor->setEnabled(true);

    //   clearData();
    return true;

seterr:
    ::close(fd);

openerr:
    return false;
}

/**
  * 功　能：关闭端口
  * 入口参数：
  * 出口参数：
  * 返回值：
  */
void Serial_Trans :: closeSerial()
{
    if(fd >= 0)
    {
        ::close(fd);
        fd = -1;

        delete monitor;
        emit serialClosed();
    }
}

/**
  * 功　能：发送数据
  * 入口参数：pBuff -> 缓冲区   nCount -> 长度
  * 返回值：发送的实际长度  <=0 出错
  */
int Serial_Trans::sendData(uchar *pBuff, int nCount, int msec)
{
    QMutexLocker locker(&mutex);
    int ret;

    for(int i = 0; i < 3; i++){ //连发三次
        ret = sendData(pBuff, nCount);
//        QByteArray array;
//        QString strArray;
//        array.append((char *)pBuff, nCount);
//        strArray = array.toHex(); // 十六进制
//        for(int i=0; i<array.size(); ++i)
//            strArray.insert(2+3*i, " "); // 插入空格
        //qDebug()<< "send:" << strArray;
        msleep(msec);
    }
    if(ret > 0) {
        if(msec > 0) msleep(msec);
    }
    return ret;
}


int Serial_Trans::sendData(uchar *pBuff, int nCount)
{
    if(fd >= 0)
    {
        // QMutexLocker locker(&mutex);
        int sendLen = write(fd, pBuff, nCount);
        fsync(fd);
        return sendLen;
    }

    return fd;
}
/**
  * 功　能：读取数据
  * 入口参数：pBuf -> 缓冲区
  * 返回值：读取的实际长度  <=0 出错
  */
int Serial_Trans::recvData(uchar *pBuf, int msecs)
{
    // QMutexLocker locker(&mutex);
    int count=0, ret=0;
    if(fd >= 0)
    {
        do
        {
            int rtn = read(fd, pBuf, 256);
            #if (SI_RTUWIFI==1)
            msleep(90);
            #endif
            if(rtn > 0) {
                pBuf += rtn; // 指针移动
                ret += rtn; // 长度增加
                count = msecs-1;
            } else {
                count++;
            }

            if(ret > 250) {
                read(fd, pBuf-250, 256);
                ret = 0; break;
            }

        } while (count < msecs);
    }
    return ret;
}

/**
  * 功　能：传输数据
  * 入口参数：sent -> 发送缓冲区, len ->  发送长度
  * 出口参数：recv -> 接收缓冲区
  * 返回值：读取的实际长度  <=0 出错
  */
int Serial_Trans::transmit(uchar *sent, int len, uchar *recv)
{
    QMutexLocker locker(&mutex);
    int ret = sendData(sent, len);
    if(ret > 0) {
        ret = recvData(recv, 5);
        //         if(ret <=0 ) qDebug() << "Serial Trans Err!!!" << ret;
    }
    return ret;
}

/**
  * 功　能：传输数据
  * 入口参数：sent -> 发送缓冲区, len ->  发送长度
  * 出口参数：recv -> 接收缓冲区
  * 返回值：读取的实际长度  <=0 出错
  */
int Serial_Trans::transmit_p(uchar *sent, int len, uchar *recv)
{
    QMutexLocker locker(&mutex);
    int ret = sendData(sent, len);

//    QByteArray array;
//    QString strArray;
//    array.append((char *)sent, len);
//    strArray = array.toHex(); // 十六进制
//    for(int i=0; i<array.size(); ++i)
//        strArray.insert(2+3*i, " "); // 插入空格
//    qDebug()<< "send:" << strArray<<ret;RTU_SENT_DC_LEN
    while(ret < RTU_SENT_LEN) {
        ret = recvData(recv, 5);
        if(ret == 5||ret == 0)
        {
           break;
        }
    }
//    array.clear();
//    strArray="";
//    array.append((char *)recv, ret);
//    strArray = array.toHex(); // 十六进制
//    for(int i=0; i<array.size(); ++i)
//        strArray.insert(2+3*i, " "); // 插入空格
//    qDebug()<< "recv:" << strArray<<ret;
    return ret;
}

void Serial_Trans::readDataSlot()
{
    emit serialReadSig();
}

/**
   * 功　能：读取设备fd
   * 返回值：
   */
int Serial_Trans::getSerialFd(void)
{
    return fd;
}
