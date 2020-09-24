/*
 * udpdataque.cpp
 * UDP 接收到的数据都会保存至此队列中
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "udpdataque.h"

static QReadWriteLock  *gLock = new QReadWriteLock();
static QQueue<UdpBaseData*> *gUdpQueueData = new QQueue<UdpBaseData*>(); // UDP套接字接收数据队列


/**
 * @brief 增加数据
 * @param data
 */
void udp_data_add(UdpBaseData *data)
{
    QWriteLocker locker(gLock);
    gUdpQueueData->enqueue(data);
}

/**
 * @brief 数据队列的大小
 * @return
 */
int udp_data_size(void)
{
    QReadLocker locker(gLock);
    return gUdpQueueData->size();
}


/**
 * @brief 获取队列数据
 * @param data
 * @return 数据的长度
 */
UdpBaseData *udp_data_get(void)
{
    UdpBaseData *data = NULL;

    QReadLocker locker(gLock); /*获取线程状态*/
    int ret = gUdpQueueData->size();
    if( ret > 0)
        data = gUdpQueueData->dequeue();

    return data;
}
