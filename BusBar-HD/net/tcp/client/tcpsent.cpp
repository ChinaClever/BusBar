/*
 * tcpclient.cpp
 * Tcp发送类 主要用于TCP数据发送
 *      1、建立连接 newConnect()
 *      2、增加要发送的数据 tcp_queue_append()
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tcpsent.h"

static QReadWriteLock  *gLock = new QReadWriteLock();
static QQueue<QByteArray> *gArrayQue = new QQueue<QByteArray>; // 需要发送的数据都会保存至此队列中

/**
 * @brief 往列队中增加数据
 * @param data
 */
void tcp_queue_append(QByteArray &data)
{
    QWriteLocker locker(gLock);
    gArrayQue->enqueue(data);
}

/**
 * @brief 增加数据
 * @param buf 缓冲区
 * @param len 数据长度
 */
void tcp_queue_append(uchar *buf, int len)
{
    QByteArray data;
    data.append((char *)buf, len);
    tcp_queue_append(data);
}

TcpSent::TcpSent(QObject *parent) : QThread(parent)
{
    mTcpClient = new TcpClient(this);

    mLock = gLock;
    mArrayQue = gArrayQue;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(800);
}


/**
 * @brief 连接服务器
 * @param host
 * @return
 */
void TcpSent::newConnect(const QString &host)
{
    if(host != mHost) {
        mHost = host;
        QWriteLocker locker(mLock);
        mArrayQue->clear();
    }

    mTcpClient->newConnect(mHost);
}

/**
 * @brief 自动连接目标主机
 */
bool TcpSent::newConnect(void)
{
    bool ret = get_tcp_connect();
    if(ret == false)
    {
        if(!mHost.isEmpty())
        {
            mTcpClient->newConnect(mHost);
//             msleep(500); // 等待连接
        }
    }

    return get_tcp_connect();
}

/**
 * @brief 发送数据进行检查
 * @return true 可以发送数据
 */
bool TcpSent::sentCheck(void)
{
    bool ret = newConnect();
    if(ret)
        ret = mTcpClient->getSentStatue();
    return ret;
}

/**
 * @brief 发送数据
 */
void TcpSent::sentData(void)
{
    if(mArrayQue->size() > 0)
    {
        QWriteLocker locker(mLock);
        QByteArray data = mArrayQue->dequeue();
        mTcpClient->sentMessage(data);
    }
}

/**
 * @brief 定时发送数据
 */
void TcpSent::timeoutDone()
{
    bool ret = sentCheck();
    if(ret) {
        sentData();
    }
}

/**
 * @brief 发送测试数据 测试用
 */
void tcp_sent_test(void)
{
    net_dev_data pkt;
    uchar lzy[512] = {0};

    pkt.addr = 0;
    pkt.fn[0] = 0;
    pkt.fn[1] = 0;
    pkt.len = 10;
    pkt.data = (uchar *)"luozhiyong";

    int ret = net_data_packets(0x01010101, 1, &pkt, lzy);

    tcp_queue_append(lzy, ret);
}
