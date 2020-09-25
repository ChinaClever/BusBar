/*
 * netanalyzedata.cpp
 * 解析网络数据包
 *      把数据包还原成对应的结构体
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "netanalyzedata.h"

/**
 * 功  能：生成校验码
 * 入口参数：pkt->包结构休
 * 返回值：校验码
 */
uchar net_data_xor(uchar *buf, ushort len)
{
    ushort i;
    uchar XOR=0;

    for(i=0; i<len; ++i)
        XOR += buf[i];

    return XOR;
}

/**
 * 功  能：同步数据包 查检数据包是否完整
 * 入口参数：pkt->包结构休
 * 返回值：同步头的位置，   -2 同步头出错 	-3结束符错误
 */
static int data_packet_sync(uchar *buf, ushort len)
{
    int ret=0,rtn = -1;
    uchar head[2] = {DATA_MSG_HDR, DATA_MSG_STX}; /*包头*/
    ushort offset=0;

    if(len < 2+DATA_MSG_CODE_SIZE+2) /*数据包不完整*/
        return DATA_ERR_ABNORMAL;
    else if(len > DATA_MSG_SIZE)
        return DATA_ERR_LEN;

    /*查找同步头*/
    do
    {
        if(offset < len-10)
        {
            ret = memcmp(buf+offset, head, sizeof(head)); /*查找包头*/
            if(ret!=0) /*不是包头*/
                offset++; /*偏量增加*/
        }
        else
            return DATA_ERR_HEAD; /*同步头错误*/
    } while(ret!=0);

    /*验证包尾*/
    rtn = offset + 2 + DATA_MSG_CODE_SIZE; /*代号段结束位置*/
    rtn += (buf[rtn]<<8) + buf[rtn+1]; /*数据长度*/
    rtn += 2+1; /*数据长度二字节、校验位一字节*/

    if(rtn < len)
    {
        if(buf[rtn] == DATA_MSG_ED) /**/
            return offset; /*返回头位置*/
    }

    return DATA_ERR_END;/*结束符未找到*/
}

/**
 * 功能：解析代号段数据
 * 入口参数：buf->数据首地址
 * 出口参数：code->打包之后MSG
 * 返回:长度
 */
static int data_packet_getCode(uchar *buf, net_dev_code *code)
{
    uchar i,*ptr=buf;

    for(i=0; i<DEV_CODE_SIZE; ++i)
        code->devCode[i] = *(ptr++); /*设备代号*/

    code->type = *(ptr++); /*通讯类型*/
    code->version = *(ptr++); /*版本号*/
    code->trans = *(ptr++); /*服务端发送标志：10H 客户端应答标志：03H */
    //code->reserve = *(ptr++);
    //code->reserve *= 256;
    //code->reserve += *(ptr++);
    code->reserve = 0;

    return DATA_MSG_CODE_SIZE;
}





/**
 * 功  能：解析数据段数据
 * 入口参数：buf->数据首地址
 * 出口参数：msg->打包之后MSG
 * 返  回:长度 	-1 出错
 */
static int data_packet_getDomain(uchar *buf, net_data_packet *msg)
{
    uchar XOR,*ptr = buf;

    int len = (ptr[0]*256) + ptr[1]; /*获取数据长度*/
    msg->len = len;
    ptr += 2;

    if((len >= 0) && (len < DATA_MSG_SIZE))
    {
        msg->data = ptr; /* 数据段*/
        ptr += len;
    }
    else
    {
        qDebug() << "data_packet_getDomain err:" << len;
        return DATA_MSG_SIZE;
    }

    msg->XOR = *(ptr++);
    XOR = net_data_xor(msg->data, len); // 检查校验码
    XOR = msg->XOR;  // 不检查校验码 为了提高执行效率  校验码不检查
    if(XOR != msg->XOR)
        return DATA_ERR_XOR;

    return (2+len+1);
}


/**
 * 功能：解析数据包
 * 入口参数：buf->数据首地址 	len->数据长度
 * 出口参数：pkt->打包之后MSG
 * 返回:  > 0 表示接收成功
 * 说明：当收到数据包时就会调用此函数把数据解包成MSG格式
 */
int net_data_analytic(uchar *buf, ushort len, net_data_packet *msg)
{
    int ret;
    uchar *ptr=buf;

    ret = data_packet_sync(buf,len); //验证包的各种特性
    if(ret >= 0)
    {
        ptr = buf+ret; /*信息头偏移距离*/
        msg->hdr = *(ptr++); /*信息头*/
        msg->stx = *(ptr++); /*标识字*/

        ptr += data_packet_getCode(ptr, &(msg->code)); /*获取代号段数据*/
        ret = data_packet_getDomain(ptr, msg);	/*获取数据段*/
        if(ret >= 0)
        {
            ptr += ret;
            msg->ED = *ptr;

            return (2+DATA_MSG_CODE_SIZE+2+msg->len+2); /*整个包的长度*/
        }
    }

    return ret;
}



/**
 * 功能：解析设备数据包
 * 入口参数：buf->数据首地址 	len->数据长度
 * 出口参数：pkt->打包之后MSG
 * 返回:TRUE
 */
int dev_data_analytic(uchar *buf, ushort len, net_dev_data *pkt)
{
    uchar *ptr=buf;

    pkt->num = *(ptr++);
    pkt->addr = *(ptr++);/*获取源地址码*/
    for(int i=0; i<DEV_FN_SIZE; ++i) /*功能码*/
        pkt->fn[i] = *(ptr++);

    ushort rtn = (*ptr) * 256 + *(ptr+1); /*数据长度*/
    if(rtn < len)
    {
        pkt->len = rtn;
        ptr += 2;

        if(pkt->len > 0) /*数据区*/
            pkt->data = ptr;
    }
    else
        rtn = 0;

    return rtn;
}


