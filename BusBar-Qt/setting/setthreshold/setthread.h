#ifndef SETTHREAD_H
#define SETTHREAD_H

#include "setnetcmd.h"

class SetThread : public QThread
{
    Q_OBJECT
    explicit SetThread(QObject *parent = nullptr);
public:
    ~SetThread();
    static SetThread *bulid(QObject *parent = nullptr);
    void append(sThresholdItem &item) {mItems.append(item);}

protected:
    void run();
    void workDown();

private:
    bool isRun;


    SetShm *mSetShm;
    SetRtuCmd *mRtuCmd;
    SetNetCmd *mNetCmd;
    QList<sThresholdItem> mItems;
};

#endif // SETTHREAD_H
