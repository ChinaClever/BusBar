/*
 * udpsentsocket.cpp
 * UDP 发送套接字
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "udpsentsocket.h"

UdpSentSocket::UdpSentSocket()
{
    mUdpSocket = NULL;
    sentLock = new QReadWriteLock();
    mUdpSocket = new QUdpSocket(0);
}


UdpSentSocket::~UdpSentSocket()
{
    delete mUdpSocket;
    delete sentLock;
}



bool UdpSentSocket::sentData(QHostAddress &addr,uchar *buf, ushort len)
{
    QByteArray byte;

    for(int i=0; i<len; ++i)
        byte.append(buf[i]);

    return sentData(addr,byte);
}

/**
 * @brief 发送点对点数据
 * @param msg 数据内容
 * @return true
 */
bool UdpSentSocket::sentData(QHostAddress &addr,QByteArray &msg)
{
    return sentData(addr,msg, UDP_SENT_PORT);
}

bool UdpSentSocket::sentData(QHostAddress &addr,QByteArray &msg, int port)
{
    bool ret = true;

    QWriteLocker locker(sentLock);
    int len = mUdpSocket->writeDatagram(msg.data(),msg.size(),addr,port);
    if(len != msg.length())
    {
        ret = false;
        qDebug() << "UdpServer SentData Err" << msg;
    }

    return ret;
}

/**
 * @brief 发送广播数据
 * @param msg 数据内容
 * @return true
 */
bool UdpSentSocket::sentBroadcastData(uchar *buf, ushort len, quint16 port)
{
    QByteArray byte;

    for(int i=0; i<len; ++i)
        byte.append(buf[i]);

    return sentBroadcastData(byte,port);
}



/**
 * @brief 发送广播数据
 * @param msg 数据内容
 * @return true
 */
bool UdpSentSocket::sentBroadcastData(QByteArray &msg, quint16 port)
{
    bool ret = true;

    QWriteLocker locker(sentLock);
    int len = mUdpSocket->writeDatagram(msg.data(),msg.size(),QHostAddress::Broadcast,port);
    if(len != msg.length())
    {
        ret = false;
        qDebug() << "UdpServer SentData Err" << msg;
    }

    return ret;
}

/***********************************C语言实现********************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>

static int sock_fd=-1; /*套按字*/
static struct sockaddr_in server_addr;

/* 创建一个socket，类型是SOCK_DGRAM，udp类型*/
static int udp_creatSocket(void)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        qDebug("udp Client Creat Socket Err\n");

    return sock;
}

/* 初始化预连接的服务端地址*/
static void udp_initServerAddr(char *server_ip, int port)
{
    char* url = server_ip;
    //printf("%s\n",url);

    struct hostent *host= (struct hostent *) gethostbyname(url); /* 通过函数入口参数url获得host地址（如果是域名，会做域名解析）*/

    /* 初始化预连接的服务端地址 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
}

static void udp_initSocket(void)
{
    if(sock_fd < 0)
    {
        sock_fd = udp_creatSocket(); // 创建套接字
    }
}

/**
 * 功能：udp 数据发送
 * 入口参数：*send_data -> 数据缓冲区  len -> 数据长度
 * 出口参数：
 * 返回: 发送长度，
 */
static int udp_clientSentData(char *ip, int port, uchar *send_data, uint len)
{
    int ret=0, sockfd=sock_fd;

    udp_initSocket();	/*服务端检查*/
    udp_initServerAddr(ip, port);

    /* 发送数据到服务远端 */
    ret = sendto(sockfd, send_data, len, 0,
                 (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

    return ret;
}

void udp_sent_data(const QString &ip, int port, uchar *buf, uint len)
{
    udp_clientSentData(ip.toLatin1().data(),port, buf, len);
}

void udp_sent_data(const QString &ip, uchar *buf, uint len)
{
   udp_sent_data(ip, 28720, buf, len);
}
