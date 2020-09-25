#ifndef LOGBTNBAR_H
#define LOGBTNBAR_H

#include <QWidget>
#include "logexportmodeldlg.h"
#include "logsignal.h"

#define SQL_DEL_MODE 1

namespace Ui {
class LogBtnBar;
}

class LogBtnBar : public QWidget
{
    Q_OBJECT

public:
    explicit LogBtnBar(QWidget *parent = 0);
    ~LogBtnBar();

signals:
    void refreshSig();
    void querySig(QString);
    void exportSig();
    void clearSig();
    void exportSig(int);

private slots:
    void on_dateBtn_clicked();
    void on_queryBtn_clicked();

    void on_exportBtn_clicked();

    void on_clearBtn_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::LogBtnBar *ui;
    LogExportModelDlg *mExportDlg;
};

#endif // LOGBTNBAR_H
