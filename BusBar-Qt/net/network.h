#ifndef NETWORK_H
#define NETWORK_H
#include "analyze/netdataanalyze.h"

class NetWork : public QObject
{
    Q_OBJECT
public:
    NetWork(QObject *parent = 0);
signals:
    void sendNetBusSig(int index);

private:
    NetDataAnalyze *mNetDataAnalyze;


};

#endif // NETWORK_H
