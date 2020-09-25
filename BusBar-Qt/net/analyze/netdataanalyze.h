#ifndef NETDATAANALYZE_H
#define NETDATAANALYZE_H
#include "udprecvsocket.h"
#include "netanalyzedata.h"
#include "udpheartbeat.h"

class NetDataAnalyze : public QThread
{    
    Q_OBJECT
public:
    explicit NetDataAnalyze(QObject *parent = 0);
    ~NetDataAnalyze();

protected:
    void initFun(void);
    void run(void);

    int recvData(void);

protected slots:
    void timeoutDone();

private:
    UdpHeartBeat *mHeartBeat;
    UdpRecvSocket *mSocket;
    bool isRun;

    QString mIP;
    uchar *mRecvBuf; //数据缓冲区
    net_data_packet *mPacket; //网络数据包
    net_dev_data *mDevData; //设备数据结构体

    pdu_devData_packet *mPduData; //设备数据包结构体
};

extern void pdu_hashData_save(pdu_devData_packet *packet);

#endif // NETDATAANALYZE_H
