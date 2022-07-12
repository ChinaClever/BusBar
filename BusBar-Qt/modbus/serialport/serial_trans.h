#ifndef SERIAL_TRANS_H
#define SERIAL_TRANS_H

/**
  * 串口基本操作 打开、关闭、读写
  */
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <QObject>
#include <qsocketnotifier.h>
//#include <sys/termios.h>
#include <termios.h>
#include <QDebug>
#include <QMutexLocker>
#include "serial_portset.h"


class Serial_Trans : public QThread
{
    Q_OBJECT
public:
    explicit Serial_Trans(QObject *parent = 0);
    ~Serial_Trans();
    
    bool openSerial(const QString);
    int getSerialFd(void);
    void closeSerial();

    int sendData(uchar *pBuff, int nCount, int msec);
    int sendData(uchar *pBuff, int nCount);
    int recvData(uchar *pBuf, int msecs);
    int transmit(uchar *sent, int len, uchar *recv);
    int transmit_p(uchar *sent, int len, uchar *recv);

protected:
    int readData(uchar *pBuf, int nCount);

signals:
    void serialReadSig();
    void serialClosed();
    void closeSerialSig();

protected  slots:
    void readDataSlot();
    void closeSerialSlot();

private:
    int fd; //串口句柄
    QMutex mutex; // 互拆锁
    QSocketNotifier * monitor;
};

#endif // SERIAL_TRANS_H
