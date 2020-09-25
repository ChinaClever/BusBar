#ifndef PASSORDSETTINGDLG_H
#define PASSORDSETTINGDLG_H

#include <QDialog>
#include "common.h"

namespace Ui {
class PassordSettingDlg;
}

class PassordSettingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PassordSettingDlg(QWidget *parent = 0);
    ~PassordSettingDlg();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    bool checkJurisdiction();

private:
    Ui::PassordSettingDlg *ui;
};

#endif // PASSORDSETTINGDLG_H
