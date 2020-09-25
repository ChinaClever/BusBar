/*
 * udpheartbeat.cpp
 * UDP 心跳包线程
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "udpheartbeat.h"

#define UDP_HEARTBEAT_MSG   "Clever IDC-BUSBAR Server OK!" // 心跳包
static QStringList mIpList;

void set_hb_IP(const QStringList &ips)
{
    mIpList = ips;
}


UdpHeartBeat::UdpHeartBeat(QObject *parent) : QThread(parent)
{
    isRun=false;
    mSocket = NULL;
}


UdpHeartBeat::~UdpHeartBeat()
{
    isRun=false;
    wait();

    delete mSocket;
}


/**
 * @brief 启动心跳包线程
 */
void UdpHeartBeat::startSent(void)
{
    mSocket = new UdpSentSocket();

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(2500);
}

void UdpHeartBeat::timeoutDone(void)
{
  start(); // 在定时器中启动线程
   //run();
}

/**
* @brief 心跳包
*/
void UdpHeartBeat::heartbeatPacket(void)
{
//    static QByteArray msg = UDP_HEARTBEAT_MSG;

    if(mSocket) {

        for(int i=0; i<mIpList.size(); ++i) {
//            QString ip = mIpList.at(i);
//            QHostAddress addr(ip);


//            mSocket->sentData(addr, msg, UDP_HB_PORT);
              udp_sent_data(mIpList.at(i), UDP_HB_PORT,
                            (uchar *)UDP_HEARTBEAT_MSG,
                            strlen(UDP_HEARTBEAT_MSG));


              sleep(1);
        }
        //        mSocket->sentBroadcastData(msg,UDP_HB_PORT);
    }
}


void UdpHeartBeat::run(void)
{
    if(isRun != true) // 在运行就不执行
    {
        isRun = true;
        heartbeatPacket();
        isRun = false;
    }
}

