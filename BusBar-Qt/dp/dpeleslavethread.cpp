/*
 * dpeleslavethread.cpp
 * 电能数据保存线程
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dpeleslavethread.h"
#include "dbbranchele.h"
#include "dbmainele.h"

DpEleSlaveThread::DpEleSlaveThread(QObject *parent) : QThread(parent)
{
    isRun = false;
    shm = get_share_mem(); // 获取共享内存

    timer = new QTimer(this);
    timer->start(24*60*60*1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}


DpEleSlaveThread::~DpEleSlaveThread()
{
    isRun = false;
    wait();
}

void DpEleSlaveThread::timeoutDone()
{
    if(isRun == false) {
        start();
    }
}

void DpEleSlaveThread::saveBox(int bus, sBoxData  &box)
{
    double rate = 10;
    if(box.offLine == 0) return; //不保存
    sObjData *data = &(box.data);

    DbBranchEleItem item;
    item.name = box.boxName;
    for(int i=0; i<9; ++i)
        item.loops[i] = data->ele[i]/rate;
    item.loop = box.tgBox.ele/rate;

    db_branchEle_obj(bus)->insertItem(item);
    msleep(50);
}


void DpEleSlaveThread::saveBus(int id)
{
    int line = 0;
    double rate = 10;

    sBusData *bus = &(shm->data[id]);
    sBoxData *busBox = &(bus->box[0]);
    if(busBox->tgBox.vol == 0) return; //不保存
    sObjData *data = &(busBox->data);

    DbMainEleItem item;
    item.line_1 = data->ele[line++]/rate;
    item.line_2 = data->ele[line++]/rate;
    item.line_3 = data->ele[line++]/rate;
    item.line = busBox->tgBox.ele/rate;
    db_mainEle_obj(id)->insertItem(item);
    msleep(50);

    static int count=0;
    if(count++ % 24 == 0)  {
        for(int i=1; i<=bus->boxNum; ++i)
            saveBox(id, bus->box[i]);
    }
}


void DpEleSlaveThread::run()
{
    if(isRun == false)
    {
        isRun  = true;

        for(int i=0; i<BUS_NUM; ++i)
            saveBus(i);

        isRun  = false;
    }
}
