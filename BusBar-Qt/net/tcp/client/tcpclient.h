#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QtCore>
#include <QtNetwork>
#include "netpackdata.h"

#define TCP_PORT	12707 // TCP端口号

class TcpClient : public QThread
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);
    ~TcpClient();

     bool newConnect(const QString &host, int port=TCP_PORT); //连接服务器
     bool sentMessage(uchar *buf,  int len);
     bool sentMessage(QByteArray &data);
     bool getSentStatue(void);

protected:
     int writeMessage(char *data, int len);
     int writeMessage(QByteArray &data);
     void closeConnect(void);

signals:
     void disconnectedSig(); // 断开信号
     void sentErr(QString);

protected slots:
    void timeoutDone(void);
    void connectedSlot();
    void disconnectedSlot();
    void readMessageSlot();  //接收数据
    void displayError(QAbstractSocket::SocketError);  //显示错误

private:
    QHostAddress *mServerIP;
    QTcpSocket *mTcpSocket;
    QByteArray mDatagram;
};

bool get_tcp_connect(void);

#endif // TCPCLIENT_H
