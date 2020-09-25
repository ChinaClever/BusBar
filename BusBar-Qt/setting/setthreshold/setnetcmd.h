#ifndef SETNETCMD_H
#define SETNETCMD_H

#include "setrtucmd.h"
#include "network.h"

class SetNetCmd : public QObject
{
    Q_OBJECT
    explicit SetNetCmd(QObject *parent = nullptr);
public:
    static SetNetCmd *bulid(QObject *parent = nullptr);

    bool send(sThresholdItem &item);
    bool sentNetData(int bus, uchar *buf, int len);

protected:
    uchar *getData(sThresholdItem &item, int rate);
    bool sentNetDatas(int bus, uchar *buf, int len);

private:
    UdpSentSocket *mSocket;
};

#endif // SETNETCMD_H
