#include "apptcpserver.h"
#include "datadone/netanalyzedata.h"

#define ANDROID_TCP_PORT	11283  // 案桌TCP端口号

extern char currentBus;

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

#include "send/devsetthread.h"

static int sock_fd=-1, gSocket=-1;

#define udp_printf qDebug
/**
 * 功 能： 创套接字
 * 参 数：port->端口，lisnum->等待连接队列的最大长度。
 * 返 回：套接字
 */
static int tcp_creatSocket(int port, int lisnum)
{
    int ret;
    struct sockaddr_in sin;//struct sockaddr结构体用来处理网络通信的地址。

    /*
     *int socket(int domain, int type, int protocol);
     * PF_INET, AF_INET： Ipv4网络协议
     * PF_INET6, AF_INET6： Ipv6网络协议。
     * type参数的作用是设置通信的协议类型，可能的取值如下所示：
            SOCK_STREAM： 提供面向连接的稳定数据传输，即TCP协议。
            OOB： 在所有数据传送前必须使用connect()来建立连接状态。
            SOCK_DGRAM： 使用不连续不可靠的数据包连接。
            SOCK_SEQPACKET： 提供连续可靠的数据包连接。
            SOCK_RAW： 提供原始网络协议存取。
            SOCK_RDM： 提供可靠的数据包连接。
            SOCK_PACKET： 与网络驱动程序直接通信。
     */
    //socket函数，向系统申请一个通信端口
    int sockfd=socket(AF_INET,SOCK_STREAM,0);//IPV4 TCP协议           ______ [1]
    if(sockfd >= 0)//申请失败
    {
        memset(&sin,0,sizeof(sin));// 初始化 然后是设置套接字
        sin.sin_family = AF_INET;//协议族，在socket编程中只能是AF_INET(TCP/IP协议族)
        sin.sin_addr.s_addr=INADDR_ANY;//sin_addr存储IP地址,使用in_addr这个数据结构
        sin.sin_port=htons(port);//存储端口号

        ret = bind(sockfd,(struct sockaddr *)&sin,sizeof(sin)); //将套接字（sin） 跟端口（sockfd）链接      ______ [2]
        if(ret >= 0)
        {
            /*int PASCAL FAR listen( SOCKET s, int backlog);
                   S：用于标识一个已捆绑未连接套接口的描述字。
                   backlog：等待连接队列的最大长度。
             * listen()仅适用于支持连接的套接口，如SOCK_STREAM类型的。
             */
            ret = listen(sockfd,lisnum);                                                  //  _______ [3]
            if(ret >= 0) //在端口sockfd监听
            {
                qDebug("TCP accepting connections \n");
                return sockfd; //OK
            }
            else
                qDebug("call to listen err\n");
        }
        else
            qDebug("call to bind\n");
    }
    else
        qDebug("call to socket err");

    return -1;
}

static int tcp_sent(void *buf, int len)
{
    if(gSocket > 0) {
        len = send(gSocket, buf, len, 0);
    } else
        len = -1;

    return len;
}

/**
 * 功 能：接收数据
 * 参 数：sockfd->套接字，buf->接收数据缓冲区, len-> 接收缓冲区长度
 */
static void tcp_recv(int sockfd)
{
    uchar buf[512] = {0};
    net_data_packet packet;
    dev_data_packet pkt;
    int ret,rtn;
    do
    {
        usleep(1);
        ret = recv(sockfd,buf,512,0); //接收
        if( ret > 0)
        {
            rtn = net_data_analytic(buf, ret, &packet); //解析数据包 _ 返回数据长度
            if(rtn >= 0)
            {
                rtn = dev_data_analytic(packet.data, packet.len, &pkt);
                if(rtn > 0)
                {
                    qDebug() << pkt.addr << pkt.fn[0] << pkt.fn[1] ;
                    DevSetThread::bulid()->insert(pkt);
                    udp_printf("recv: %d %s\n", pkt.len, pkt.data);
                }
                else
                    udp_printf("recv err %d\n", rtn);
            } else
                udp_printf("net_data_analytic err %d\n", rtn);
        } else
            udp_printf("call to recv err %d \n", ret);
    }while(ret > 0);

    close(sockfd);
    gSocket = -1;
}


/**
 * 首先进行身份验证
 */
static int landVerify(int sockfd)
{
    char buf[512] = {0};
    int ret = recv(sockfd,buf,256,0);
    if(ret > 0) {
        udp_printf("recv land verify : %d %s\n", ret, buf);
        currentBus = buf[ret-1]; //全局母线
        gSocket = sockfd;
        android_sent((uchar *)"OK",2);
    }

    return 1;
}

/**
 * 功 能：接收客户端连接
 *
 */
static int tcp_accept(int sockfd)
{
    struct sockaddr_in pin;

    int size = sizeof(struct sockaddr_in);
    uint sock = accept(sockfd,(struct sockaddr *)&pin, &size);  //                   ______ [4]
    if(sock >= 0)
    {
        udp_printf("Client IP: %s\n", inet_ntoa(pin.sin_addr));
        if(landVerify(sock) > 0)
            tcp_recv(sock);
    }
    else {
        gSocket = -1;
        udp_printf("call to accept error\n");
        sleep(1);
    }

    return 0;
}

AppTcpServer::AppTcpServer(QObject *parent) : QThread(parent)
{
    isRun = true;
    sock_fd = tcp_creatSocket(ANDROID_TCP_PORT, 1);
}

AppTcpServer *AppTcpServer::bulid()
{
    static AppTcpServer* sington = NULL;
    if(sington == NULL)
        sington = new AppTcpServer();
    return sington;
}

AppTcpServer::~AppTcpServer()
{
    isRun = false;
    wait();
}

void AppTcpServer::sent(uchar *data, int len)
{
    tcp_sent(data, len);
    msleep(4);
}

void AppTcpServer::run()
{
    while (isRun) {
        tcp_accept(sock_fd);
    }
}

void android_sent(uchar *buf, int len)
{
    AppTcpServer::bulid()->sent(buf, len);
}
