#include "systemsettingdlg.h"
#include "ui_systemsettingdlg.h"
#include <time.h>

SystemSettingDlg::SystemSettingDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemSettingDlg)
{
    ui->setupUi(this);
    //mEmailWid = new EmailWid(ui->smtpWid);
#if ( SI_RTUWIFI == 0 )
    mIpWid = new IpSettingDlg(ui->smtpWid);
#elif ( SI_RTUWIFI == 1)
    mChannelWid = new ChannelSettingDlg(ui->smtpWid);
    connect(this, SIGNAL(busChangedSig(int)), mChannelWid, SLOT(busChangedSlot(int)));
#endif
    mNetWid = new NetSettingDlg(ui->netWid);
    mOtherWid = new OtherSettingDlg(ui->otherWid);
}

SystemSettingDlg::~SystemSettingDlg()
{
    delete ui;
}



