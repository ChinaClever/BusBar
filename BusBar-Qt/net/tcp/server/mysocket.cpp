#include "mysocket.h"

MySocket::MySocket(int sockDesc, QObject *parent) :
    QTcpSocket(parent),
    m_sockDesc(sockDesc)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(recvData()));
}

MySocket::~MySocket()
{

}

void MySocket::sendData(int id, const char *data , int len)
{
    if (id == m_sockDesc && len != 0) {
        this->write(data , len);
        this->flush();
    }
}

void MySocket::recvData(void)
{
    QString ip = peerAddress().toString().remove(0, 7);
    QByteArray data = readAll();

    emit dataReady(ip, data);
}
