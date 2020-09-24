/*
 * devDataSent.c
 *
 *  Created on: 2016年7月29日
 *      Author: Lzy
 */
#include "devDataSent.h"
#include "apptcpserver.h"
#include <QDebug>
#include "common/datapacket.h"
#include "common/common.h"

extern char currentBus;
static uchar gSentBuf[DATA_MSG_SIZE]={0};


static int data_msg_packetSent(uchar *buf, ushort len)
{
    android_sent(buf,len);
    //    len = udp_clientSentData(gSentBuf,len);
    return len;
}

static int dev_code_data(void)
{
    return 0x02010101;
}

static int data_packet_sent(dev_data_packet *msg)
{
    uchar *buf = gSentBuf;
    int type =TRA_TYPR_TCP;
    int num = dev_code_data();

    memset(buf,0,DATA_MSG_SIZE);
    ushort rtn = net_data_packets(num, type,msg, buf);
    rtn = data_msg_packetSent(buf, rtn);

    return rtn;
}

/**
 * 发送工作状态
 */
void sent_devStatus(int id, int addr, sBoxData *box)
{
    uchar buf[4], len=0;

    buf[len++] = box->boxStatus;
    buf[len++] = box->dc;
    buf[len++] = box->version;
    buf[len++] = box->loopNum;

    dev_data_packet msg;
    msg.num = id;
    msg.addr = addr;

    msg.len = len;  //=======
    msg.data = buf;

    msg.fn[0] = 0;
    msg.fn[1] = 0;
    data_packet_sent(&msg);
}


/**
 * 发送数据包
 *  发送数据的条件：数据长度大于0 缓冲区地址有效
 */
static void sent_packet(dev_data_packet *msg)
{
    if(msg->len > 0) // 必需有数据
    {
        if(msg->data) // 数据指针有效
            data_packet_sent(msg);
    }
}

/**
 * 功  能：短整形变成字符串型数据
 * 入口参数：from-> 数据来源，len-> 数据长度
 * 出口参数：to->数据缓冲区
 * 返  回：数据长度
 */
static int shortToChar(ushort *form, int len, uchar *to)
{
    int i, offset=0;
    if(form)
    {
        memset(to,0, DATA_MSG_SIZE);
        for(i=0; i<len; ++i)
        {
            to[offset++] = form[i] >> 8; // 高位
            to[offset++] = form[i] & 0xFF; // 低8位
        }
    }

    return offset;
}

/**
 * 功  能：整形变成字符串型数据
 * 入口参数：from-> 数据来源，len-> 数据长度
 * 出口参数：to->数据缓冲区
 * 返  回：数据长度
 */
static int intToChar(uint *form, int len, uchar *to)
{
    int i, offset=0;

    if(form)
    {
        memset(to,0, DATA_MSG_SIZE);
        for(i=0; i<len; ++i)
        {
            to[offset++] = form[i] >> 24; // 高位
            to[offset++] = (form[i] >> 16) & 0xFF;
            to[offset++] = (form[i] >> 8) & 0xFF;
            to[offset++] = form[i] & 0xFF; // 低8位
        }
    }

    return offset;
}

/**
 * 功  能：发送数据单元数据
 * 参  数：unit->数据单元结构体
 * 		  buf->发送数据缓冲区
 * 		  len-> 数据长度
 */
static void sent_unit(_devDataUnit *unit, int len, uchar *buf, dev_data_packet *msg)
{
    int fn=1, fc = msg->fn[1];

    /* 发送当前值 */
    msg->fn[1] = fc + fn++;
    msg->data = buf;
    msg->len = shortToChar(unit->value,len,buf);
    sent_packet(msg);

    /*发送最小值*/
    msg->fn[1] = fc + fn++;
    msg->len = shortToChar(unit->min,len,buf);
    sent_packet(msg);

    /*发送最大值*/
    msg->fn[1] = fc + fn++;
    msg->len = shortToChar(unit->max,len,buf);
    sent_packet(msg);

    /*发送告警*/
    msg->fn[1] = fc + fn++;
    msg->len = len;
    msg->data = unit->alarm;
    sent_packet(msg);

    /*发送临界最小值*/
    msg->fn[1] = fc + fn++;
    msg->data = buf;
    msg->len = shortToChar(unit->crMin,len,buf);
    sent_packet(msg);

    /*发送临界最大值*/
    msg->fn[1] = fc + fn++;
    msg->data = buf;
    msg->len = shortToChar(unit->crMax,len,buf);
    sent_packet(msg);

    /*发送临界告警*/
    msg->fn[1] = fc + fn++;
    msg->len = len;
    msg->data = unit->crAlarm;
    sent_packet(msg);
}

/**
 * 功 能：发送数据对象 包括电流、电压、功率、电能、功率因素等
 */
static void sent_object(_devDataObj *obj, uchar *buf, dev_data_packet *msg)
{
    int fn=0, len = obj->len;

    /*电流*/
    fn += 1;
    msg->fn[1] = fn << 4;
    sent_unit(&(obj->cur), len, buf, msg);

    /*电压*/
    fn += 1;
    msg->fn[1] = fn << 4;
    sent_unit(&(obj->vol), len, buf, msg);

    /*功率*/
    fn += 1;
    msg->fn[1] = fn << 4;
    msg->data = buf;
    msg->len = intToChar(obj->pow,len,buf); // 功率
    sent_packet(msg);

    /*电能*/
    fn += 1;
    msg->fn[1] = fn << 4;
    msg->data = buf;
    msg->len = intToChar(obj->ele,len,buf); // 电能
    sent_packet(msg);

    /*功率因素*/
    fn += 1;
    msg->fn[1] = fn << 4;
    msg->data = buf;
    msg->len = shortToChar(obj->pf,len,buf);
    sent_packet(msg);

    /*开关控制*/
    fn += 1;
    msg->fn[1] = fn << 4;
    msg->data = obj->sw;
    msg->len = len;
    sent_packet(msg);

    fn += 1;
    msg->fn[1] = fn << 4;
    msg->data = buf;
    msg->len = shortToChar(obj->apPow,len,buf);
    sent_packet(msg);

    /*电压频率*/
    fn += 1;
    msg->fn[1] = fn << 4;
    msg->data = buf;
    msg->len = shortToChar(obj->rate,len,buf); // 功率
    sent_packet(msg);

    /**/
    if(len>3) len=3;
    fn += 1;
    msg->fn[1] = fn << 4;
    msg->data = buf;
    msg->len = shortToChar(obj->pl,len,buf);
    sent_packet(msg);

    fn += 1;
    msg->fn[1] = fn << 4;
    msg->data = buf;
    msg->len = shortToChar(obj->curThd,len,buf);
    sent_packet(msg);

    fn += 1;
    msg->fn[1] = fn << 4;
    msg->data = buf;
    msg->len = shortToChar(obj->volThd,len,buf);
    sent_packet(msg);
}

/**
 * 功  能：发送温度、湿度、门禁、水浸等相关数据
 */
static void sent_envObject(_envDataObjct *obj, uchar *buf, dev_data_packet *msg)
{
    int fn=0, len = obj->len;

    /*湿度*/
    fn += 1;
    msg->fn[1] = fn << 4;
    sent_unit(&(obj->tem), len, buf, msg);

    /*湿度*/
    fn += 1;
    msg->fn[1] = fn << 4;
    sent_unit(&(obj->hum), len, buf, msg);

    /*门禁*/
    fn += 1;
    msg->fn[1] = fn << 4;
    msg->data = obj->door;
    msg->len = 2;
    sent_packet(msg);

    /*水禁*/
    fn += 1;
    msg->fn[1] = fn << 4;
    msg->data = obj->water;
    msg->len = 1;
    sent_packet(msg);

    /*烟雾*/
    fn += 1;
    msg->fn[1] = fn << 4;
    msg->data = obj->smoke;
    msg->len = 1;
    sent_packet(msg);
}

/**
 * 功  能：设备数据发送
 */
void sent_devData(uchar id, int box, pduDevData *devData)
{
    static dev_data_packet msg;
    static uchar buf[DATA_MSG_SIZE] = {0};
    int fn=1;

    msg.num = id;
    msg.addr = box;
    msg.fn[0] = fn++;
    sent_object(&(devData->line),buf, &msg);

    msg.fn[0] = fn++;
    sent_object(&(devData->loop),buf,&msg);

    msg.fn[0] = fn++;
    //    sent_object(&(devData->output),buf,&msg);

    msg.fn[0] = fn++;
    sent_envObject(&(devData->env), buf,&msg);
}




/**
 * 初始化数据  Unit，
 */
void init_Unit(_devDataUnit *unit, sDataUnit *cUnit)
{
    unit->value = cUnit->value;
    unit->max = cUnit->max;
    unit->min = cUnit->min;
    unit->alarm = cUnit->alarm;

    unit->crMin = cUnit->crMin;
    unit->crMax = cUnit->crMax;
    unit->crAlarm = cUnit->crAlarm;
}

/**
 * 数据初始化  -- 相
 */
void init_dataLoop(_devDataObj *ptr, sObjData *obj)
{
    ptr->len = obj->lineNum;
    init_Unit(&(ptr->vol), &(obj->vol));
    init_Unit(&(ptr->cur), &(obj->cur));

    ptr->sw    = obj->sw;   //开关状态
    ptr->pow   = obj->pow;  // 功率
    ptr->ele   = obj->ele;  // 电能
    ptr->pf    = obj->pf;   //功率因素
    ptr->apPow = obj->apPow; //视在功率
    ptr->pl  = obj->pl;  // 谐波值
    ptr->curThd  = obj->curThd;
    ptr->volThd  = obj->volThd;
}

void init_dataLine(_devDataObj *ptr, sLineTgObjData *obj, sObjData *p)
{
    ptr->len = 3;
    ptr->vol.value = obj->vol;
    ptr->cur.value = obj->cur;
    ptr->pow   = obj->pow;  // 功率
    ptr->ele   = obj->ele;  // 电能
    ptr->pf    = obj->pf;   //功率因素
    ptr->apPow = obj->apPow; //视在功率

    ptr->pl  = p->pl;  // 谐波值
    ptr->curThd  = p->curThd;
    ptr->volThd  = p->volThd;
}


void sent_str(int id, int fn1, int fn2, short len, char *str)
{
    dev_data_packet msg;
    msg.addr = id;
    msg.len = len;
    msg.data = (uchar *)str;

    msg.fn[0] = fn1;
    msg.fn[1] = fn2;
    data_packet_sent(&msg);
}

void sent_busName(sBusData *bus)
{
    dev_data_packet msg;
    msg.num = 0;
    msg.addr = 0;

    char *nameBuf = bus->busName;
    msg.len = strlen(nameBuf);
    msg.data = (uchar *)nameBuf;

    msg.fn[0] = 5;
    msg.fn[1] = 0x11;
    data_packet_sent(&msg);
}

void sent_loopName(int id,sBoxData *box)
{
    for(int i=0; i<box->loopNum; ++i) {
        char *nameBuf = box->loopName[i];
        int len = strlen(nameBuf);
        sent_str(id, 10, i, len,  nameBuf);
    }
}

void sent_busRateCur(int id, sBusData *bus)
{
    static uchar rateCurBuf[2] = {1};
    rateCurBuf[0] = (uchar)(bus->box[0].ratedCur >> 8);
    rateCurBuf[1] = (uchar)(bus->box[0].ratedCur);

    dev_data_packet msg;
    msg.num = id;
    msg.addr = 0;

    msg.len = 2;  //=======
    msg.data = rateCurBuf;

    msg.fn[0] = 30;
    msg.fn[1] = 0;
    data_packet_sent(&msg);
}

void sent_busBoxNum(int id, sBusData *bus)
{
    static uchar boxNumBuf[2] = {2};
    boxNumBuf[0] = bus->boxNum;
    boxNumBuf[1] = bus->boxNum;

    dev_data_packet msg;
    msg.num = id;
    msg.addr = 0;

    msg.len = 1;  //=======
    msg.data = boxNumBuf;

    msg.fn[0] = 31;
    msg.fn[1] = 0;
    data_packet_sent(&msg);
}

void sent_busThdData(int id, sThdData *thdData)
{
    static uchar buf[DATA_MSG_SIZE] = {0};

    dev_data_packet msg;
    msg.num = id;
    msg.addr = 0;
    msg.data = buf;
    msg.fn[0] = 3;

    for(int i=0; i<3; ++i) {
        msg.fn[1] = 1 << 4;
        msg.fn[1] += i;
        msg.len = shortToChar(thdData->volThd[i],30,buf);
        data_packet_sent(&msg);
    }

    for(int i=0; i<3; ++i) {
        msg.fn[1] = 2 << 4;
        msg.fn[1] += i;
        msg.len = shortToChar(thdData->curThd[i],30,buf);
        data_packet_sent(&msg);
    }
}

/**
 * 发送测试数据， 测试用
 */
void sent_dev_data(int index)
{
    //uchar id = currentBus - '0';
    int id = index;
    sDataPacket *shm = get_share_mem(); // 获取共享内存
    static pduDevData *devData = nullptr;
    if(!devData) devData = (pduDevData*)malloc(sizeof(pduDevData)); //申请内存

    int len = shm->data[index].boxNum + 1;  //始端箱也算
    for(int  i=0; i< len; ++i) {

        memset(devData, 0, sizeof(pduDevData));
        sBoxData *box =  &(shm->data[id].box[i]);

        if(box->offLine < 1) continue; //不在线就跳过
        else if(box->boxSpec) box->offLine--;

        init_dataLoop(&(devData->loop), &(box->data));
        init_dataLine(&(devData->line), &(box->lineTgBox), &(box->data));

        devData->env.len = SENSOR_NUM;
        init_Unit(&(devData->env.tem), &(box->env.tem)); //温度

        sent_devData(id, i,devData);
        sent_devStatus(id, i, box);
        sent_loopName(id, box);

        //sent_str(i, 6, 0x11, strlen(str), str);
        //free(devData); //释放
    }
    sent_busName(&shm->data[id]);
    sent_busRateCur(id, &shm->data[id]);
    sent_busBoxNum(id, &shm->data[id]);
    sent_busThdData(id, &shm->data[id].thdData);
}






