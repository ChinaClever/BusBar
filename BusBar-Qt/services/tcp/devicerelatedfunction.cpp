#include "devicerelatedfunction.h"

bool landVerify(QByteArray array)
{
    QString str = QString(array);
    if(str == "admin; admin; 0")
        return true;
    return false;
}

void reply_to_client()
{
    int ret;
    char buf[12] = "ok";
    BusTcpServer *server = getTcpServer();
    if(server->isConnecting())
        ret = server->sendData(buf);
    qDebug() << "=============ret==============" << ret;
}
