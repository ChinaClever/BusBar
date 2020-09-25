#ifndef LOGBRANCHELEEXPORTTHREAD_H
#define LOGBRANCHELEEXPORTTHREAD_H

#include <QObject>
#include "excel/excelexportwid.h"
#include "dbbranchele.h"

class LogBranchEleExportThread : public QThread
{
    Q_OBJECT
public:
    explicit LogBranchEleExportThread(QObject *parent = 0);
    ~LogBranchEleExportThread();

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
    DbBranchEle* mEle;
    sExcelExportStr *mExcelStr;
};
extern sExcelExportStr *gExcelExportStr;

#endif // LOGBRANCHELEEXPORTTHREAD_H
