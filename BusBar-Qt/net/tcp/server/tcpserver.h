#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "rtuthread.h"

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

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);

    void init(int port, bool isVerify=true);
    bool isConnect();

    void sendData(char *data);
    void sendData(uchar *data, int len);
    void sendData(const QByteArray &data);

    int readData(QString &ip, char *data);

signals:

protected:
    void landVerify(QTcpSocket *socket);
    void transData(uchar *buf, int len);
    bool validateData(int rtn);
    void setCrc(uchar *buf, int len);
    uint calcZeroCur(sBoxData *box);
    uchar rtu_sent_to_input_packet(sBoxData *box);
    uchar rtu_sent_to_output_packet(sBoxData *box);

private slots:
    void newConnectSlot();
    void readMessage();
    void removeUserFormList();


private:
    QTcpServer *m_tcpServer;
    QMap<QString, QTcpSocket *> m_mapClient;
    QString mIP;
    bool mIsConnect, mIsVerify;
    bool isRun;
    uchar *mBuf;
    ThrNetData *mThr;
    sDataPacket *mShm;
    sBoxData *mBox;
};

#endif // TCPSERVER_H
