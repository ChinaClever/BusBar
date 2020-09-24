#ifndef DPALARMSLAVE_H
#define DPALARMSLAVE_H

#include <QObject>
#include "common.h"

class DpAlarmSlave : public QThread
{
    Q_OBJECT
public:
    explicit DpAlarmSlave(QObject *parent = 0);
    ~DpAlarmSlave();

signals:

protected:
    void run();
    void unitAlarm(QString &typeStr, QString &msg, sDataUnit &unit, double rate, const QString &sym);
    void unitAlarmVA(sBoxData &box, QString &typeStr, QString &msg, sDataUnit &unit, double rate, const QString &sym);

    void boxAlarm(sBoxData &box);
    void busAlarm(int id);
    void checkAlarm();

   void saveMsg(const QString &typeStr, const QString &str);

protected slots:
    void timeoutDone();

private:
     bool isRun;
     QTimer *timer;
     sDataPacket *shm;
     int mBusId;
     QStringList mAlarmStr;
};

QString get_email_str();
QStringList get_alarm_str();
#endif // DPALARMSLAVE_H
