#ifndef NETSETTINGDLG_H
#define NETSETTINGDLG_H

#include <QWidget>
#include <QNetworkInterface>

namespace Ui {
class NetSettingDlg;
}

class NetSettingDlg : public QWidget
{
    Q_OBJECT

public:
    explicit NetSettingDlg(QWidget *parent = 0);
    ~NetSettingDlg();

private slots:
    void on_pushButton_4_clicked();

private:
    void initData();

private:
    Ui::NetSettingDlg *ui;
};

#endif // NETSETTINGDLG_H
