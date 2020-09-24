#ifndef SETMAINWID_H
#define SETMAINWID_H

#include "setnames/setnameswid.h"
#include "setloop/settemmain.h"
#include "setsystem/systemsettingdlg.h"
#include "setsystem/checkpassworddlg.h"
namespace Ui {
class SetMainWid;
}

class SetMainWid : public QWidget
{
    Q_OBJECT

public:
    explicit SetMainWid(QWidget *parent = 0);
    ~SetMainWid();

private:
    void initWidget();
    void setButtonColor(QPushButton *button);

public slots:
    void busChangedSlot(int);

private slots:
    void initFunSLot();
    void on_nameBtn_clicked();
    void on_lineBtn_clicked();
    void on_loopBtn_clicked();
    void on_temBtn_clicked();
    void on_sysBtn_clicked();
#if ( SI_RTUWIFI == 1 )
signals:
    void busChangedSig(int);
#endif

private:
    Ui::SetMainWid *ui;

    SetTemMain *mSetTemWid;
    SetNamesWid *mSetNamesWid;
    SetLineWid *mSetLineWid;
    SetLoopWid *mSetLoopWid;
    SystemSettingDlg *mSystemDlg;
};

#endif // SETMAINWID_H
