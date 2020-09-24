#ifndef OTHERSETTINGDLG_H
#define OTHERSETTINGDLG_H

#include <QWidget>
#include "passordsettingdlg.h"

namespace Ui {
class OtherSettingDlg;
}

class OtherSettingDlg : public QWidget
{
    Q_OBJECT

public:
    explicit OtherSettingDlg(QWidget *parent = 0);
    ~OtherSettingDlg();

private slots:
    void on_updateBtn_clicked();
    void on_resetBtn_clicked();
    void on_timeSetBtn_clicked();
    void on_pwdSetBtn_clicked();

private:
    Ui::OtherSettingDlg *ui;
};

#endif // OTHERSETTINGDLG_H
