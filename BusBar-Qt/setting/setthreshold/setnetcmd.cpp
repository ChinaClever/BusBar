#include "setnetcmd.h"

extern QString getIpByKey(int key);
extern int net_data_packets(net_dev_data *pkt, uchar *buf);

SetNetCmd::SetNetCmd(QObject *parent) : QObject(parent)
{
    mSocket = new UdpSentSocket();
}

SetNetCmd *SetNetCmd::bulid(QObject *parent)
{
    static SetNetCmd* sington = NULL;
    if(sington == NULL) {
        sington = new SetNetCmd(parent);
    }
    return sington;
}

/**
 * @brief 发送网络模式数据
 * @return
 */
bool SetNetCmd::sentNetData(int bus, uchar *buf, int len)
{
    bool ret = false;
    QString ip = getIpByKey(bus);
    if(!ip.isEmpty()) {
        QHostAddress addr(ip);
        ret = mSocket->sentData(addr, buf, len);
    }

    return ret;
}

bool SetNetCmd::sentNetDatas(int bus, uchar *buf, int len)
{
    bool ret = false;
    if(bus == 0xff)
    {
        for(int i=0; i<BUS_NUM; ++i)
            ret |= sentNetData(i, buf, len);
    } else {
        ret = sentNetData(bus, buf, len);
    }

    return ret;
}

uchar *SetNetCmd::getData(sThresholdItem &item, int rate)
{
    static uchar buf[16] = {0};
    int offset=0;

    int min = item.min * rate;
    buf[offset++] = (min >> 8)&0xFF;
    buf[offset++] = min & 0xFF;

    int max = item.max * rate;
    buf[offset++] = (max >> 8)&0xFF;
    buf[offset++] = max & 0xFF;

    return buf;
}

bool SetNetCmd::send(sThresholdItem &item)
{
    static uchar array[64] = {0};
    int rate = 1;
    net_dev_data pkt;

    pkt.num = item.bus; // 母线
    pkt.addr = item.box; // 插接箱
    pkt.fn[0] = item.type; // 1 电压 2 电流 3温度
    if(item.type==2) rate = 10;

    int line = item.num;
    if(item.box == 0xff||item.bus ==0xff) line = 0xff;
    pkt.fn[1] = line; // 相

    pkt.len = 4;
    pkt.data = getData(item, rate);
    int len = net_data_packets(&pkt, array);

    return sentNetDatas(item.bus, array, len);
}
