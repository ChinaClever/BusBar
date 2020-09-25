#ifndef LOGMAINELEEXPORTDLG_H
#define LOGMAINELEEXPORTDLG_H

#include <QDialog>
#include "logmaineleexportthread.h"

namespace Ui {
class LogMainEleExportDlg;
}

class LogMainEleExportDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LogMainEleExportDlg(QWidget *parent = 0);
    ~LogMainEleExportDlg();

    void set(int id);

protected slots:
     void readDbSlot(QString);
     void exportSlot(bool);
     void overSlot();

private slots:
     void on_pushButton_clicked();

private:
    Ui::LogMainEleExportDlg *ui;
    LogMainEleExportThread *mDbThread;
    ExcelExportWid *mProgressWid;
};

#endif // LOGMAINELEEXPORTDLG_H
