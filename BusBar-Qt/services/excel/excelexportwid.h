#ifndef EXCELEXPORTWID_H
#define EXCELEXPORTWID_H

#include <QWidget>
#include "txtexportlogthread.h"
#include "common.h"

namespace Ui {
class ExcelExportWid;
}

class ExcelExportWid : public QWidget
{
    Q_OBJECT

public:
    explicit ExcelExportWid(QWidget *parent = 0);
    ~ExcelExportWid();
    void set(const QString &path, const QString &file, const QList<QStringList> &list, int mode=1);

signals:
    void overSig();


protected slots:
    void progressSlot(int);
    void overSlot(bool);

private:
    Ui::ExcelExportWid *ui;
    ExcelExportLogThread *mExportThread;
    TxtExportLogThread *mtxtThread;
};

#endif // EXCELEXPORTWID_H
