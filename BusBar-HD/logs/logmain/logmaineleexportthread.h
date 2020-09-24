#ifndef LOGMAINELEEXPORTTHREAD_H
#define LOGMAINELEEXPORTTHREAD_H

#include <QObject>
#include "excel/excelexportwid.h"
#include "dbmainele.h"

class LogMainEleExportThread : public QThread
{
    Q_OBJECT
public:
    explicit LogMainEleExportThread(QObject *parent = 0);
    ~LogMainEleExportThread();

    QList<QStringList> mList;
    void initData(int busId);

signals:
    void readDbSig(QString);
    void exportSig(bool);

protected:
    void run();
    bool readDb();

signals:

public slots:

private:
    DbMainEle* mEle;
    sExcelExportStr *mExcelStr;
};
extern sExcelExportStr *gExcelExportStr;

#endif // LOGMAINELEEXPORTTHREAD_H
