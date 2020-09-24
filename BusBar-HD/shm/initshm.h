#ifndef INITSHM_H
#define INITSHM_H

#include <QObject>
#include "common.h"

class InitShm : public QThread
{
    Q_OBJECT
public:
    explicit InitShm(QObject *parent = 0);

signals:

protected:
    void run();
    void initBoxNum();

    void initThresholdUnit(int id, sDataUnit &unit, int max);
    void initBoxThreshold();
    void initBusThreshold();
    void initThreshold();

    void initNameUnit(int bus, int type, int num, char *buf, const QString &name);
    void initBusName();
    void initBoxName();
    void initLoopName();
    void initName();

private:
    sDataPacket *shm;
};

#endif // INITSHM_H
