#ifndef LOGALARMELEEXPORTDLG_H
#define LOGALARMELEEXPORTDLG_H

#include <QDialog>
#include "logalarmexportthread.h"
#include "excel/excelexportwid.h"

namespace Ui {
class LogAlarmEleExportDlg;
}

class LogAlarmEleExportDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LogAlarmEleExportDlg(QWidget *parent = 0);
    ~LogAlarmEleExportDlg();

    void set(int id);

protected slots:
     void readDbSlot(QString);
     void exportSlot(bool);
     void overSlot();

private slots:
     void on_pushButton_clicked();

private:
    Ui::LogAlarmEleExportDlg *ui;
    LogAlarmExportThread *mDbThread;
    ExcelExportWid *mProgressWid;
};

#endif // LOGALARMELEEXPORTDLG_H
