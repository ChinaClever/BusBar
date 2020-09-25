/*
 * dpthread.cpp
 * 数据处理线程入口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dpthread.h"

DpThread::DpThread(QObject *parent) : QThread(parent)
{
    QTimer::singleShot(5*1000,this,SLOT(initFunSLot())); //延时初始化
}

DpThread::~DpThread()
{

}


void DpThread::initFunSLot()
{
    mTgThread = new DpTgThread(this);
    mAlarmThread = new DpAlarmThread(this);
    mEle = new DpEleSlaveThread(this);
    mAlarmSlave = new DpAlarmSlave(this);
    mBeep = new DpBeepThread(this);
}





