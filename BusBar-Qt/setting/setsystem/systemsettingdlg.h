#ifndef SYSTEMSETTINGDLG_H
#define SYSTEMSETTINGDLG_H

#include <QWidget>
#include <cstdlib>
#include <QDateTime>
#include <QDebug>
#include "common.h"
#include "smtp/emailwid.h"
#include "netsettingdlg.h"
#include "othersettingdlg.h"
#include "ipsettingdlg.h"
#if (SI_RTUWIFI == 1)
#include "channelsettingdlg.h"
#endif

namespace Ui {
class SystemSettingDlg;
}

class SystemSettingDlg : public QWidget
{
    Q_OBJECT

public:
    explicit SystemSettingDlg(QWidget *parent = 0);
    ~SystemSettingDlg();
    #if (SI_RTUWIFI == 1)
signals:
    void busChangedSig(int);
    #endif

private:
    Ui::SystemSettingDlg *ui;
    EmailWid *mEmailWid;
    NetSettingDlg *mNetWid;
    OtherSettingDlg *mOtherWid;
    IpSettingDlg* mIpWid;
    #if (SI_RTUWIFI == 1)
    ChannelSettingDlg* mChannelWid;
    #endif
};

#endif // SYSTEMSETTINGDLG_H
