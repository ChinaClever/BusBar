#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "bustcpserver.h"
#include "devicerelatedfunction.h"

class TcpThread : public QThread
{
    Q_OBJECT
public:
    TcpThread(QObject *parent = 0);

protected:
    void run();

private:
    bool is_run;
    BusTcpServer *mServer;
    QByteArray data;

private slots:
    void timeoutdone();
};

#endif // TCPTHREAD_H
