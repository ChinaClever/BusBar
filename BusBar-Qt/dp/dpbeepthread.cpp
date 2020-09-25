#include "dpbeepthread.h"
#include "beepthread.h"

DpBeepThread::DpBeepThread(QObject *parent) : QThread(parent)
{
    isRun = false;
    shm = get_share_mem(); // 获取共享内存

    timer = new QTimer(this);
    timer->start(1*1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}


DpBeepThread::~DpBeepThread()
{
    isRun = false;
    wait();
}

void DpBeepThread::timeoutDone()
{
    if(isRun == false) {
        start();
    }
}


void DpBeepThread::boxAlarm(sBoxData &box)
{
    if((box.boxVolAlarm == 1)  || (box.boxCurAlarm == 1)  || (box.boxEnvAlarm ==1))
    {
        BeepThread::bulid()->beep();
    }
}


void DpBeepThread::busAlarm(sBusData &bus)
{
    for(int i=0; i<=bus.boxNum; ++i) {
        boxAlarm(bus.box[i]);
    }   
}

void DpBeepThread::run()
{
    if(isRun == false)
    {
        isRun  = true;

        for(int i=0; i<BUS_NUM; ++i)
            busAlarm(shm->data[i]);

        isRun  = false;
    }
}
