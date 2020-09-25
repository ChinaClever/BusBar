#ifndef DEVSETTHREAD_H
#define DEVSETTHREAD_H

#include <QObject>
#include <QMap>
#include <QThread>

#include "netdataformat.h"
#include "setthreshold/setthread.h"

class DevSetThread : public QThread
{
    Q_OBJECT
    explicit DevSetThread(QObject *parent = nullptr);
public:
    static DevSetThread* bulid();

    void insert(const dev_data_packet &pkt);
};

#endif // DEVSETTHREAD_H
