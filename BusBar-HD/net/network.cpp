#include "network.h"
#include "send/netsendthread.h"

char currentBus = '0';

//void set_hb_IP(const QStringList &ips);

NetWork::NetWork(QObject *parent) : QObject(parent)
{
//    QStringList ips;
//    ips << "192.168.1.164";
//    set_hb_IP(ips); // 设置目标IP ，也就是把心跳1包发给住
    mNetDataAnalyze = new NetDataAnalyze(this); // 接收IP 模块数据

    NetSendThread::bulid()->start(); //平板端连接
    connect(this , SIGNAL(sendNetBusSig(int)) , NetSendThread::bulid() , SLOT(recvBusSlot(int)));
}
