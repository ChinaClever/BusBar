#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

#include "serverthread.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = Q_NULLPTR);
    ~Server();

private:
    void incomingConnection(int sockDesc);
    QList<int> mSocketList;
};

#endif // SERVER_H
