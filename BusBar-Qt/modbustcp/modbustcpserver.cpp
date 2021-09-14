#include "modbustcpserver.h"
#include <QDebug>

ModbusTcpServer::ModbusTcpServer(int port)
{
    m_Port = port;
}

int ModbusTcpServer::run()
{
    if (this->listen(QHostAddress::Any,m_Port))
    {
        qDebug()<<"[TcpServer]-------------------------------------------------listen sucess"<<endl;
        return 1;
    }
    else
    {
        qDebug()<<"[TcpServer]-------------------------------------------------listen faile"<<endl;
        return 0;
    }
}

void ModbusTcpServer::incomingConnection(int socketDescriptor)
{
     qDebug()<<"[TcpServer]------------------------------------------new Connection !!! The Num:"<< tcpSocketConnetList.count() + 1<<endl;

     TcpSocket *tcpsocket = new TcpSocket();
     tcpsocket->setSocketDescriptor(socketDescriptor);
     tcpsocket->run();
     connect(tcpsocket,SIGNAL(GetDataFromClient(QByteArray ,int)),this,SLOT(Scoket_Data_Processing(QByteArray,int)));
     connect(tcpsocket,SIGNAL(ClientDisConnected(int)),this,SLOT(Socket_Disconnected(int)));
     tcpSocketConnetList.append(tcpsocket);
     //qDebug()<<"[TcpServer]-------------------------:"<<tcpSocketConnetList.count()<<endl;
     //qDebug()<<"[TcpServer]-------------------------:"<<tcpSocketConnetList.size()<<endl;
}


void ModbusTcpServer::Scoket_Data_Processing(QByteArray SendData,int descriptor)
{
    for(int i = 0; i < tcpSocketConnetList.count(); i++)
    {
        QTcpSocket *item = tcpSocketConnetList.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            qDebug()<<"From ---> "<< item->peerAddress().toString() <<":"<<item->peerPort();
            qDebug()<<"[SendData]:"<< SendData ;
            qDebug()<<"End --- "<< endl;       
        }
    }
}

void ModbusTcpServer::Socket_Disconnected(int descriptor)
{
    for(int i = 0; i < tcpSocketConnetList.count(); i++)
    {
        QTcpSocket *item = tcpSocketConnetList.at(i);
        int temp = item->socketDescriptor();
        if(-1 == temp||temp == descriptor)            //
        {
            tcpSocketConnetList.removeAt(i);//如果有客户端断开连接， 就将列表中的套接字删除
            item->deleteLater();
            qDebug()<< "[TcpSocket]---------------------------------Disconnect:" << descriptor << endl;
            return;
        }
    }
    return;
}
