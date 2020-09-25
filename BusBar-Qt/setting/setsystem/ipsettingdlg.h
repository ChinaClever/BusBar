#ifndef IPSETTINGDLG_H
#define IPSETTINGDLG_H

#include <QWidget>
#include "common.h"
#include "net/udp/sent/udpheartbeat.h"
#define IP1 "192.168.1.163"

namespace Ui {
class IpSettingDlg;
}

class IpSettingDlg : public QWidget
{
    Q_OBJECT

public:
    explicit IpSettingDlg(QWidget *parent = 0);
    ~IpSettingDlg();

protected:
    void initData();
    bool ipCheck(int i, QLineEdit *edit);

private slots:
    void on_saveBtn_clicked();

private:
    Ui::IpSettingDlg *ui;
};
int getByIp(const QString &ip);
#endif // IPSETTINGDLG_H
