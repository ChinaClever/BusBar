#ifndef THDMAINDLG_H
#define THDMAINDLG_H

#include <QDialog>
#include "thdchartwid.h"
#include "thdtablewid.h"

namespace Ui {
class ThdMainDlg;
}

class ThdMainDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ThdMainDlg(QWidget *parent = 0);
    ~ThdMainDlg();

    void initBus(int id);

protected:
    void setLine(int i, int l);

private slots:
    void timeoutDone();
    void on_volAbtn_clicked();
    void on_volBbtn_clicked();
    void on_volCbtn_clicked();
    void on_curAbtn_clicked();
    void on_curBbtn_clicked();
    void on_curCbtn_clicked();
    void on_quitBtn_clicked();

private:
    Ui::ThdMainDlg *ui;
    QTimer *timer;
    ushort *mThd;
    ushort *mLineThd;
    sBusData *mBusData;

    ThdChartWid *mChartWid;
    ThdTableWid *mTableWid;
};

#endif // THDMAINDLG_H
