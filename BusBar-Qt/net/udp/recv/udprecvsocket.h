#ifndef UDPRECVSOCKET_H
#define UDPRECVSOCKET_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>


class UdpRecvSocket
{
public:
    explicit UdpRecvSocket();

    int recvData(QString &ip, uchar *buf);
};

#endif // UDPRECVSOCKET_H
