#ifndef DPELESLAVETHREAD_H
#define DPELESLAVETHREAD_H

#include <QtSql>
#include <QObject>
#include "common.h"

class DpEleSlaveThread : public QThread
{
    Q_OBJECT
public:
    explicit DpEleSlaveThread(QObject *parent = 0);
    ~DpEleSlaveThread();
    class DB_Tran
    {
        public:
        DB_Tran() {QSqlDatabase::database().transaction();}
        ~DB_Tran() {QSqlDatabase::database().commit();}
    };
signals:

protected:
    void run();
    void saveBus(int id);
    void saveBox(int bus, sBoxData &box);

protected slots:
    void timeoutDone();

private:
    bool isRun;
    QTimer *timer;
    sDataPacket *shm;
};

#endif // DPELESLAVETHREAD_H
