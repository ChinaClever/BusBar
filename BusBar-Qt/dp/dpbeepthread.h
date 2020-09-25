#ifndef DPBEEPTHREAD_H
#define DPBEEPTHREAD_H

#include "dpalarmthread.h"

class DpBeepThread : public QThread
{
    Q_OBJECT
public:
    explicit DpBeepThread(QObject *parent = nullptr);
    ~DpBeepThread();

signals:

protected:
    void run();
    void boxAlarm(sBoxData &box);
    void busAlarm(sBusData &bus);

protected slots:
    void timeoutDone();

private:
     bool isRun;
     QTimer *timer;
     sDataPacket *shm;
};

#endif // DPBEEPTHREAD_H
