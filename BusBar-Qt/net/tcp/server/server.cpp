#include "server.h"

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
}

Server::~Server()
{
}

void Server::incomingConnection(int sockDesc)
{
    serverThread *thread = new serverThread(sockDesc);
    mSocketList.append(sockDesc);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
