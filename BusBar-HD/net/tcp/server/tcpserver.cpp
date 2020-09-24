#include "tcpserver.h"

#define ANDROID_TCP_PORT	11283  // 案桌TCP端口号

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    m_tcpServer = new QTcpServer();
    //设置最大允许连接数，不设置的话默认为30
    m_tcpServer->setMaxPendingConnections(2000);
    connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnectSlot()));
}

void TcpServer::init(int port, bool isVerify)
{
    if(m_tcpServer->listen(QHostAddress::Any, port)){
        qDebug() << "listen OK!";
    }else{
        qDebug() << "listen error!";
    }
    mIsVerify = isVerify;
}

void TcpServer::newConnectSlot()
{
    QTcpSocket *tcp = m_tcpServer->nextPendingConnection();
    if(mIsVerify) {
        connect(tcp,SIGNAL(readyRead()),this,SLOT(readMessage()));
        mIsConnect = false;
    } else {
         mIsConnect = true;
    }
    m_mapClient.insert(tcp->peerAddress().toString(), tcp);
    connect(tcp,SIGNAL(disconnected()),this,SLOT(removeUserFormList()));
    mIP = tcp->peerAddress().toString();

    //    m_pMsgHandler->devOnline(tcp->peerAddress().toString());
}

bool TcpServer::isConnect()
{
    bool ret = false;
    if(m_mapClient.contains(mIP))
       ret = mIsConnect;
    return ret;
}

void TcpServer::sendData(char *data)
{
    if(isConnect())
        m_mapClient.value(mIP)->write(QByteArray(data));
}

void TcpServer::sendData(uchar *data, int len)
{
    if(isConnect())
        m_mapClient.value(mIP)->write((char *)data, len);
}

void TcpServer::sendData(const QByteArray &data)
{
    if(isConnect())
        m_mapClient.value(mIP)->write(data);
}

int TcpServer::readData(QString &ip, char *data)
{
    int ret=0;

    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    if(socket) {
        ip = socket->peerAddress().toString();
        ret = socket->read(data, 256);
    }
    return ret;
}

/**
 * 首先进行身份验证
 */
void TcpServer::landVerify(QTcpSocket *socket)
{
    qDebug() <<socket->peerAddress().toString() << QString(socket->readAll());
    ////////////////============ 验证用户名和密码

    m_mapClient.value(mIP)->write("OK");
    mIsConnect = true;
}

void TcpServer::readMessage()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    landVerify(socket);

    disconnect(socket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    //    m_pMsgHandler->recvFromServer(socket->peerAddress().toString(), QString(socket->readAll()));
}

void TcpServer::removeUserFormList()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    QMap<QString, QTcpSocket *>::iterator it;

    for(it=m_mapClient.begin();it!=m_mapClient.end();it++)
    {
        if(socket->peerAddress().toString() == it.key())
        {
            m_mapClient.erase(it);
//            m_pMsgHandler->devOffline(socket->peerAddress().toString());

            break;
        }
    }
}
