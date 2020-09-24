/*
 * dptgthread.cpp
 * 数据统计
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dptgthread.h"

DpTgThread::DpTgThread(QObject *parent) : QThread(parent)
{
    isRun = false;
    shm = get_share_mem(); // 获取共享内存

    timer = new QTimer(this);
    timer->start(3*1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

DpTgThread::~DpTgThread()
{
    isRun = false;
    wait();
}

void DpTgThread::timeoutDone()
{
    if(isRun == false) {
        start();
    }
}


/**
 * @brief 获取平均数据 0视无效数据
 * @param data
 */
int DpTgThread::averData(ushort *data, int len)
{
    int size=0, value=0,temp;

    for(int i=0; i< len; ++i)
    {
        temp = data[i];
        if(temp > 0) {
            size++;
            value += temp;
        } else
            temp = 0;
    }

    if(size > 0)
        value /= size;

    return value;
}


void DpTgThread::tgObj(sObjData *obj, sTgObjData *tg)
{
    memset(tg, 0, sizeof(sTgObjData));
    for(int i=0; i<LINE_NUM_MAX; ++i)
    {
        tg->cur += obj->cur.value[i];
        tg->pow += obj->pow[i];
        tg->ele += obj->ele[i];
        tg->apPow += obj->apPow[i];
    }
    tg->vol = averData(obj->vol.value);
    if(tg->apPow > 0)
        tg->pf = (tg->pow * 100.0 / tg->apPow);
    else
        tg->pf = 0;
    if(tg->pf>99) tg->pf = 99;
    //    tg->pf = averData(obj->pf);
}


void DpTgThread::lineTgObj(sObjData *obj, sLineTgObjData *tg)
{
    memset(tg, 0, sizeof(sLineTgObjData));
    for(int i=0; i<3; ++i)
    {
        tg->vol[i] = obj->vol.value[i];
        for(int j=0; j<3; ++j) {
            tg->cur[i] += obj->cur.value[i+j*3];
            tg->pow[i] += obj->pow[i+j*3];
            tg->ele[i] += obj->ele[i+j*3];
          //  tg->apPow[i] += obj->apPow[i+j*3];
              tg->apPow[i] += obj->cur.value[i+j*3] * obj->vol.value[i+j*3]/10;
        }
    }

    for(int i=0; i<3; ++i) {        
        if(tg->apPow[i] > 0) tg->pf[i] = (tg->pow[i] * 100.0 / tg->apPow[i]);
        else tg->pf[i] = 0;
        if(tg->pf[i]>99) tg->pf[i] = 99;
        if(tg->pow[i] > tg->apPow[i]) tg->pow[i] = tg->apPow[i];
    }
}


void DpTgThread::dcLineTgObj(sObjData *obj, sLineTgObjData *tg, int line, int len)
{
    memset(tg, 0, sizeof(sLineTgObjData));

    if(len == 2 && line == 2){ //er fen er
        for(int i = 0 ; i < 2; i++){
            tg->vol[i] = obj->vol.value[i];
            tg->cur[i] = obj->cur.value[i];
            tg->pow[i] = obj->pow[i];
            tg->ele[i] = obj->ele[i];
            tg->apPow[i] = obj->apPow[i];
        }
    }else{
        for(int i = 0 ; i < 2; i++){
            tg->vol[i] = obj->vol.value[i];
            for(int j=0; j<2; ++j) {
                tg->cur[i] += obj->cur.value[i*2+j];
                tg->pow[i] += obj->pow[i*2+j];
                tg->ele[i] += obj->ele[i*2+j];
                tg->apPow[i] += obj->apPow[i*2+j];
            }
        }
    }

    for(int i=0; i<2; ++i) {
        if(tg->apPow[i] > 0)
            tg->pf[i] = (tg->pow[i] * 100.0 / tg->apPow[i]);
        else
            tg->pf[i] = 0;
        if(tg->pf[i]>99) tg->pf[i] = 99;
        if(tg->pow[i] > tg->apPow[i]) tg->pow[i] = tg->apPow[i];
    }
}

void DpTgThread::tgBox(sBoxData *box)
{
    sObjData *loop = &(box->data);
    sTgObjData *tgBox = &(box->tgBox);
    sLineTgObjData *linTgBox = &(box->lineTgBox);

    if(box->offLine) {
        tgObj(loop, tgBox);
        if(box->dc) {
            lineTgObj(loop, linTgBox);
        } else  {
            dcLineTgObj(loop, linTgBox, box->rate, box->loopNum);
        }

        tgBox->tem = averData(box->env.tem.value);
    }else {
        memset(tgBox, 0, sizeof(sTgObjData));
        memset(linTgBox, 0, sizeof(sLineTgObjData));
        memset(&(box->env), 0, sizeof(sEnvData));
    }
}

void DpTgThread::tgBus(sBusData *bus)
{
    for(int i=0; i<=bus->boxNum; ++i) { // 插接箱统计
        tgBox(&(bus->box[i]));
    }
}


void DpTgThread::run()
{
    if(isRun == false)
    {
        isRun  = true;

        for(int i=0; i<BUS_NUM; ++i)
            tgBus(&(shm->data[i]));

        isRun  = false;
    }
}
