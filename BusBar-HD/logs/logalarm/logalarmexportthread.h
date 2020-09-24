#ifndef LOGALARMEXPORTTHREAD_H
#define LOGALARMEXPORTTHREAD_H

#include <QObject>
#include "excel/excelexportwid.h"
#include "dbalarm.h"

class LogAlarmExportThread : public QThread
{
    Q_OBJECT
public:
    explicit LogAlarmExportThread(QObject *parent = 0);
    ~LogAlarmExportThread();

    QList<QStringList> mList;
    void initData(int busId);

signals:
    void readDbSig(QString);
    void exportSig(bool);

protected:
    void run();
    bool readDb();

private:
    DbAlarm* mEle;
    sExcelExportStr *mExcelStr;
};
extern sExcelExportStr *gExcelExportStr;

#endif // LOGALARMEXPORTTHREAD_H
