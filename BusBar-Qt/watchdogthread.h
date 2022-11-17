#ifndef WATCHDOGTHREAD_H
#define WATCHDOGTHREAD_H
#include <QThread>

class Watchdogthread: public QThread
{
    Q_OBJECT
public:
    explicit Watchdogthread(QObject *parent = 0);

protected:
    void seedWatchdog();
    void watchdogDone();
    void clearCache();
    void clearCacheDone();
    void run();
};

#endif // WATCHDOGTHREAD_H
