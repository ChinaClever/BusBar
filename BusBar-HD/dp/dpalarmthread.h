#ifndef DPALARMTHREAD_H
#define DPALARMTHREAD_H

#include "dptgthread.h"

class DpAlarmThread : public QThread
{
    Q_OBJECT
public:
    explicit DpAlarmThread(QObject *parent = 0);
    ~DpAlarmThread();

signals:

protected:
    void run();
    void alarmDataUnit(sDataUnit &unit, int lineNum, bool cr=false);
    char alarmFlag(sDataUnit &unit, int line, bool cr=false);

    void boxAlarm(sBoxData &box);
    void busAlarm(sBusData &bus);

protected slots:
    void timeoutDone();
    void initFunSLot();

private:
     bool isRun;
     QTimer *timer;
     sDataPacket *shm;
};

#endif // DPALARMTHREAD_H
