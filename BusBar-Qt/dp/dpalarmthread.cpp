/*
 * dpalarmthread.cpp
 * 告警标志位判定线程
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dpalarmthread.h"

DpAlarmThread::DpAlarmThread(QObject *parent) : QThread(parent)
{
    isRun = false;
    shm = get_share_mem(); // 获取共享内存
    QTimer::singleShot(15*1000,this,SLOT(initFunSLot())); //延时初始化
}

DpAlarmThread::~DpAlarmThread()
{
    isRun = false;
    wait();
}

void DpAlarmThread::initFunSLot()
{
    timer = new QTimer(this);
    timer->start(3*1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

void DpAlarmThread::timeoutDone()
{
    if(isRun == false) {
        start();
    }
}



void DpAlarmThread::alarmDataUnit(sDataUnit &unit, int lineNum, bool cr)
{
    for(int i=0; i<lineNum; ++i)
    {
        if((unit.value[i] < unit.min[i]) || (unit.value[i] > unit.max[i]))
        {
            if(unit.alarm[i] == 0)
                unit.alarm[i] = 1;
        } else
            unit.alarm[i] = 0;

        if(cr) {
            if((unit.value[i] < unit.crMin[i]) || (unit.value[i] > unit.crMax[i]))
            {
                if(unit.crAlarm[i] == 0)
                    unit.crAlarm[i] = 1;
            } else
                unit.crAlarm[i] = 0;
        }
    }
}


char DpAlarmThread::alarmFlag(sDataUnit &unit, int line, bool cr)
{
    char flag=0;

    for(int i=0; i<line; ++i) {
        flag += unit.alarm[i];
        if(flag) return 2;
    }

    for(int i=0; i<line; ++i) {
        if(cr) flag += unit.crAlarm[i];
        if(flag) return 1;
    }

    return flag;
}

void DpAlarmThread::alarmDataUnit(sDataPowUnit &unit, int lineNum, bool cr)
{
    for(int i=0; i<lineNum; ++i)
    {
        if((unit.value[i] < unit.min[i]) || (unit.value[i] > unit.max[i]))
        {
            if(unit.alarm[i] == 0)
                unit.alarm[i] = 1;
        } else
            unit.alarm[i] = 0;

        if(cr) {
            if((unit.value[i] < unit.crMin[i]) || (unit.value[i] > unit.crMax[i]))
            {
                if(unit.crAlarm[i] == 0)
                    unit.crAlarm[i] = 1;
            } else
                unit.crAlarm[i] = 0;
        }
    }
}


char DpAlarmThread::alarmFlag(sDataPowUnit &unit, int line, bool cr)
{
    char flag=0;

    for(int i=0; i<line; ++i) {
        flag += unit.alarm[i];
        if(flag) return 2;
    }

    for(int i=0; i<line; ++i) {
        if(cr) flag += unit.crAlarm[i];
        if(flag) return 1;
    }

    return flag;
}

void DpAlarmThread::boxAlarm(sBoxData &box)
{
    if(box.offLine > 0) {
        int lineNum = box.loopNum;
        alarmDataUnit(box.data.cur, lineNum); // 回路是否有告警
        box.boxCurAlarm = alarmFlag(box.data.cur, lineNum);

        alarmDataUnit(box.data.vol, lineNum); // 回路是否有告警
        box.boxVolAlarm = alarmFlag(box.data.vol, lineNum);

        alarmDataUnit(box.data.pow, lineNum);
        box.boxPowerAlarm =  alarmFlag(box.data.pow, lineNum);

        //--------------[限制存在才告警]----------------- By_MW 2018.3.23
        if(box.dc){ //交流
            lineNum = 4;
        }else{ //直流
            lineNum = box.rate;
        }
        //---------------------------------------------

        alarmDataUnit(box.env.tem, lineNum);
        box.boxEnvAlarm =  alarmFlag(box.env.tem, lineNum);



        if((box.rate < box.minRate) || (box.rate > box.maxRate))
        {
            if(box.HzAlarm == 0)
                box.HzAlarm = 1;
            else
                box.HzAlarm = 2;
        } else
            box.HzAlarm = 0;

        box.boxAlarm = box.boxCurAlarm + box.boxVolAlarm + box.boxEnvAlarm + box.boxPowerAlarm + box.HzAlarm;
    } else {
        box.boxAlarm = 0;
    }
}

void DpAlarmThread::busAlarm(sBusData &bus)
{
    for(int i=0; i<=bus.boxNum; ++i) {
        boxAlarm(bus.box[i]);
    }
}


void DpAlarmThread::run()
{
    if(isRun == false)
    {
        isRun  = true;

        for(int i=0; i<BUS_NUM; ++i)
            busAlarm(shm->data[i]);

        isRun  = false;
    }
}
