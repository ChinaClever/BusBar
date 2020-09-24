#ifndef DPTHREAD_H
#define DPTHREAD_H

#include <QThread>
#include "common.h"
#include "dpalarmthread.h"
#include "dpeleslavethread.h"
#include "dpalarmslave.h"
#include "dpbeepthread.h"

class DpThread : public QThread
{
    Q_OBJECT
public:
    explicit DpThread(QObject *parent = 0);
    ~DpThread();

signals:

protected slots:
    void initFunSLot();

private:
     DpTgThread *mTgThread;
     DpAlarmThread *mAlarmThread;
     DpEleSlaveThread *mEle;
     DpAlarmSlave *mAlarmSlave;
     DpBeepThread *mBeep;
};

#endif // DPTHREAD_H
