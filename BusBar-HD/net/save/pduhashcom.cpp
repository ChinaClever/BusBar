/*
 * pduhashcom.cpp
 * PDU设备数据保存公共接口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduhashcom.h"

/**
 * @brief 把uchar型数组中的设备类型号 转化成int数据
 * @param prt
 * @return
 */
int get_pdu_devCode(uchar *prt)
{
    int devCode = 0;
    for(int i=0; i<DEV_CODE_SIZE; ++i)
    {
        devCode <<= 8;
        devCode += prt[i];
    }

    return devCode;
}

/**
 * @brief 网络数据包验证
 * @param type  传输类型 必须是UDP模式
 * @param trans 传输方面 必须来源于客户端口
 * @return
 */
bool pdu_netData_check(uchar type, uchar trans)
{
    bool ret = false;

    if(type == NET_UDP_TYPE) //必须是UDP模式
    {
        if(trans == DATA_MSG_CLIENT) //必须来源于客户端
            ret = true;
        else
            qDebug() << "net udp data not form client";
    }
    else
        qDebug() << "net transfer type not udp";

    return ret;
}

/**
 * @brief 字符符转为整形
 * @param dest 整形缓冲区
 * @param len  数据长度
 * @param buf
 * @param size 几位表示一个整数
 * @return
 */
static int uchar_to_int(uint *dest, ushort len, uchar *buf, int size)
{
    int i=0,j=0,k=0,temp=0;

    if(len%size == 0) //必须是偶数
    {
        for(i=0; i<len/size; ++i)
        {
            temp = 0;
            for(j=0; j<size; ++j)
            {
                temp <<= 8;
                temp += buf[k++];
            }
            dest[i] = temp;
        }
    }
    else
    {
        qDebug() << "char_to_int err" << len;
        len = 0;
    }

    return len/size;
}

static int uchar_to_short(ushort *dest, ushort len, uchar *buf, int size)
{
    int i=0,j=0,k=0,temp=0;

    if(len%size == 0) //必须是偶数
    {
        for(i=0; i<len/size; ++i)
        {
            temp = 0;
            for(j=0; j<size; ++j)
            {
                temp <<= 8;
                temp += buf[k++];
            }
            dest[i] = temp;
        }
    }
    else
    {
        qDebug() << "char_to_int err" << len;
        len = 0;
    }

    return len/size;
}

/**
 * @brief 把数据以Int保存至对应的数据域
 * @param ptr 数据指针
 * @param len ovr
 * @param data
 * @param sizeBit
 */
int pdu_saveHash_toData(uint *ptr, ushort len, uchar *data, int sizeBit)
{
    static uint buf[512] = {0};

    int rtn = uchar_to_int(buf, len, data, sizeBit); // 数据转化
    if(rtn > 0)
    {
        for(int i=0; i<rtn; ++i) {
            //            ptr->set(i, buf[i]); //数据保存
            ptr[i] = buf[i];
        }
    }

    return rtn;
}

int pdu_saveHash_toData(ushort *ptr, ushort len, uchar *data, int sizeBit)
{
    static ushort buf[512] = {0};

    int rtn = uchar_to_short(buf, len, data, sizeBit); // 数据转化
    if(rtn > 0)
    {
        for(int i=0; i<rtn; ++i) {
            //            ptr->set(i, buf[i]); //数据保存
            ptr[i] = buf[i];
        }
    }

    return rtn;
}


int pdu_saveHash_toData(uchar *ptr, ushort len, uchar *data, int sizeBit)
{
    for(int i=0; i<len; ++i) {
        ptr[i] = data[i];
    }

    return len;
}

bool char_to_string(QString &str, uchar *data, ushort len)
{
    static char strBuf[512] = {0};
    bool ret = true;

    if(len < sizeof(strBuf))
    {
        memset(strBuf,0, sizeof(strBuf));
        for(int i=0; i<len; ++i)
            strBuf[i] = data[i];
        str.append(strBuf);
    }
    else
        ret = false;

    return ret;
}

/**
 * @brief 保存字符串数据
 * @param str
 * @param len
 * @param data
 */
void pdu_saveHash_string(char *strBase, ushort len, uchar *data)
{
    QString str;
    bool ret = char_to_string(str,data,len);
    if(ret) {
        //        strBase->set(str);
        //////================
    }
}

/**
 * @brief 保存字符串数据
 * @param name
 * @param data
 */
void pdu_devStr_save(char *strBase, pdu_dev_data *data)
{
    pdu_saveHash_string(strBase, data->len, data->data);
}


