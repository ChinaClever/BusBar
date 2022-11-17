#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QObject>
#include <QtNetwork>
#include <QDebug>

#include "mysocket.h"
#include "rtuthread.h"
//#include "tcpserver.h"

class Socket;

enum {
    Fn_NetGet = 3  //获取数据
   ,Fn_NetSet = 0x10 //设置数据
};
extern RtuThread *rtu[4];
struct ThrNetData {
    uchar addr; // 表示从机地址码 addr%64 - 那条母线_0起  addr%4 - 那条接插相_0起 [0 - 255]
    uchar fn; // 表示功能码
    ushort position; //地址地址位
    ushort data; // get表示数据字节数_set表示设置数据
    ushort crc; // 检验码
};

class serverThread : public QThread
{
    Q_OBJECT
public:
    serverThread(int sockDesc, QObject *parent = Q_NULLPTR);
    ~serverThread();

private:
    void run(void);
    bool validateData(int rtn);
    void setCrc(uchar *buf, int len);
    void transData(uchar *buf, int len);

public slots:
    void sendDataSlot(int sockDesc, const char *data , int len);

signals:
    void dataReady(const QString &ip, const QByteArray &data);
    void sendData(int sockDesc, const char *data , int len);

private slots:   
    void recvDataSlot(const QString &ip, const QByteArray &data);
    void disconnectToHost(void);

private:
    MySocket *m_socket;

    int m_sockDesc;
    uchar *mBuf;
    uchar *mSendBuf;
    ThrNetData *mThr;
    sDataPacket *mShm;
};

#endif // SERVERTHREAD_H
