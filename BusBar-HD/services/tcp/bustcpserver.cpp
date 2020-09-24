#include "bustcpserver.h"

static BusTcpServer* server = NULL;
BusTcpServer* getTcpServer()
{
    if(!server)
    {
        server = new BusTcpServer;
    }
    return server;
}

BusTcpServer::BusTcpServer()
{
    qDebug()<<tr("==============");
    isInconnected = false;
    int port = ANDROID_TCP_PORT;
    this->listen(QHostAddress::Any ,port); //監聽來自任何地址的port端口
}

/**
 * @brief 重写incomingConnection，服务器在接收到新连接时会触发
 * @param socketDescriptor
 */
void BusTcpServer::incomingConnection(int socketDescriptor)
{
    qDebug()<<tr("contacting");
    socket = new TcpSocket;
    connect(socket,SIGNAL(socketDisconnected()),this,SLOT(disconnected())); //如果socket断开，那么发送信号
    connect(socket,SIGNAL(dataComing(QByteArray)),this,SLOT(readData(QByteArray)));
    socket->setSocketDescriptor(socketDescriptor); //初始化套接字，如果初始化有效，則返回true,否則返回false
    qDebug()<<tr("contact success");
    isInconnected = true;
}

void BusTcpServer::disconnected()
{
    isInconnected = false;
    qDebug()<<"服务器中socket断开";
}

void BusTcpServer::readData(QByteArray str)
{
    //    qDebug()<<"服务器中的数据："<<str.size();
    mData = str;

}

QByteArray BusTcpServer::getData()
{
    QByteArray array = mData;
    mData.clear();
    return array;
}

bool BusTcpServer::isConnecting()
{
    return isInconnected;
}

int BusTcpServer::sendData(QByteArray &array)
{
    int ret = socket->write(array);
    return ret;
}

int BusTcpServer::sendData(char *data)
{
    int ret = socket->write(data);
    return ret;
}
