#ifndef THIRDTHREAD_H
#define THIRDTHREAD_H

#include <QThread>
#include "serialport/serial_trans.h"
#include "rtu485/rtu_recv.h"
#include "common.h"
#include "rtuthread.h"
#include "net/tcp/server/tcpserver.h"


enum {
    Fn_Get = 3  //获取数据
   ,Fn_Set = 0x10 //设置数据
};

extern RtuThread *rtu[4];
struct ThrData {
    uchar addr; // 表示从机地址码 addr%64 - 那条母线_0起  addr%4 - 那条接插相_0起 [0 - 255]
    uchar fn; // 表示功能码
    ushort position; //地址地址位
    ushort data; // get表示数据字节数_set表示设置数据
    ushort crc; // 检验码
};

class ThirdThread : public QThread
{
    Q_OBJECT
public:
    explicit ThirdThread(QObject *parent = 0);

     bool init(const QString& name, const QString &name2);

signals:

public slots:

protected slots:
    void timeoutDone();

protected:
    void run();
    void transData();
    bool validateData(int rtn);
    void setCrc(uchar *buf, int len);
    uint calcZeroCur(sBoxData *box);
    uchar rtu_sent_to_input_packet(sBoxData *box);
    uchar rtu_sent_to_output_packet(sBoxData *box);

private:
    Serial_Trans *mSerial;
    bool isRun;
    uchar *mBuf;
    ThrData *mThr;
    sDataPacket *mShm;
    QTimer *timer;
    QString serialName1;
    QString serialName2;
    bool isOpen;
    TcpServer *mTcpServer;
};

#endif // THIRDTHREAD_H
