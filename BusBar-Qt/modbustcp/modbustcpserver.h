#ifndef MODBUSTCPSERVER_H
#define MODBUSTCPSERVER_H

#include <QTcpServer>
#include "tcpsocket.h"

class ModbusTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    ModbusTcpServer(int port=502);
    int run();

protected:
    //函数重载
    void incomingConnection(int socketDescriptor);

private:
    QList<QTcpSocket*> tcpSocketConnetList;
    int m_Port;

protected slots:
    void Scoket_Data_Processing(QByteArray SendData, int descriptor);            //处理数据
    void Socket_Disconnected(int descriptor);                                //断开连接处理


};

#endif //MODBUSTCPSERVER_H
