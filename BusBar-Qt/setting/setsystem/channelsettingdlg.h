#ifndef CHANNELSETTINGDLG_H
#define CHANNELSETTINGDLG_H

#include <QWidget>
#include "common.h"
#include "modbus/rtuthread.h"
#define CH1 "435"
namespace Ui {
class ChannelSettingDlg;
}

class ChannelSettingDlg : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelSettingDlg(QWidget *parent = 0);
    ~ChannelSettingDlg();

public slots:
    void busChangedSlot(int index);

protected:
    void initData();
    bool chCheck(int i, QLineEdit *edit);
    bool repeatCheck(int index, QString str);

private slots:
    void on_saveBtn_clicked();

private:
    Ui::ChannelSettingDlg *ui;
    int mIndex;
};
int getByCh(const QString &ch);
void getHZ(int& key);
#endif // CHANNELSETTINGDLG_H


