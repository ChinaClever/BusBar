#ifndef UDPSENTSOCKET_H
#define UDPSENTSOCKET_H

#include <QtCore>
#include <QtNetwork>

#define UDP_HB_PORT     18725 /*UDP心跳端口*/
#define UDP_SENT_PORT   18750 /*UDP发送端口*/

class UdpSentSocket
{
public:
    explicit UdpSentSocket();
    ~UdpSentSocket();

    bool sentData(QHostAddress &addr,uchar *buf, ushort len);
    bool sentData(QHostAddress &addr,QByteArray &msg);
    bool sentData(QHostAddress &addr,QByteArray &msg, int port);

    bool sentBroadcastData(uchar *buf, ushort len, quint16 port);
    bool sentBroadcastData(QByteArray &msg, quint16 port);

    QUdpSocket *mUdpSocket;
private:

    QReadWriteLock  *sentLock;
};

void udp_sent_data(const QString &ip, int port, uchar *buf, uint len);
void udp_sent_data(const QString &ip, uchar *buf, uint len);
#endif // UDPSENTSOCKET_H
