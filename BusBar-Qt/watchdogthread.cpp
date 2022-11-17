#include "watchdogthread.h"
#include <QDateTime>
#include <QDebug>

Watchdogthread::Watchdogthread(QObject *parent)
    : QThread(parent)
{

}

void Watchdogthread::seedWatchdog()
{
    system(QString("echo 1 > /dev/watchdog\n").toLatin1().data());
}

void Watchdogthread::watchdogDone()
{
    seedWatchdog();
}

void Watchdogthread::clearCache()
{
    system(QString("sync\n").toLatin1().data());
    system(QString("echo 3 > /proc/sys/vm/drop_caches\n").toLatin1().data());
}

void Watchdogthread::clearCacheDone()
{
    clearCache();
}

void Watchdogthread::run()
{
    static uint count = 0;
    while(true)
    {
        count++;
        watchdogDone();
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        qDebug()<<time << "             "<<count;
        sleep(1);
        if( count == 2*24*60*60*1000-5*60*1000){
            clearCacheDone();
            count = 0;
        }
    }
}
