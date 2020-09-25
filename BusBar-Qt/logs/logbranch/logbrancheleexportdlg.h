#ifndef LOGBRANCHELEEXPORTDLG_H
#define LOGBRANCHELEEXPORTDLG_H

#include <QDialog>
#include "logbrancheleexportthread.h"

namespace Ui {
class LogBranchEleExportDlg;
}

class LogBranchEleExportDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LogBranchEleExportDlg(QWidget *parent = 0);
    ~LogBranchEleExportDlg();

    void set(int id);

protected slots:
     void readDbSlot(QString);
     void exportSlot(bool);
     void overSlot();

private slots:
     void on_pushButton_clicked();

private:
    Ui::LogBranchEleExportDlg *ui;
    LogBranchEleExportThread *mDbThread;
    ExcelExportWid *mProgressWid;
};

#endif // LOGBRANCHELEEXPORTDLG_H
