#ifndef TXTEXPORTLOGTHREAD_H
#define TXTEXPORTLOGTHREAD_H

#include <QObject>
#include "excelexportlogthread.h"

class TxtExportLogThread : public ExcelExportLogThread
{
    Q_OBJECT
public:
    explicit TxtExportLogThread(QObject *parent = 0);


protected:
    void exportMsg(QList<QStringList> &list);
};

#endif // TXTEXPORTLOGTHREAD_H
