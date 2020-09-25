#include "ipsettingdlg.h"
#include "ui_ipsettingdlg.h"


QMap<int, QString> gBusIPMap;


int getByIp(const QString &ip)
{
    for(int i = 0 ; i < BUS_NUM ; ++i)
    {

        if(gBusIPMap[i] == ip)
            return i;
    }
    return -1;
}

QString getIpByKey(int key)
{
    return gBusIPMap[key];
}

IpSettingDlg::IpSettingDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IpSettingDlg)
{
    ui->setupUi(this);

    initData();
}

IpSettingDlg::~IpSettingDlg()
{
    delete ui;
}

void IpSettingDlg::initData()
{
    bool ret = sys_configFile_open();
    if(ret)
    {
        QStringList ips;
        QString str = sys_configFile_readStr("IP1");
        if(!str.isEmpty()) {
            ips << str;
            gBusIPMap.insert(0,str);
            ui->IP1lineEdit->setText(str);
        }

        str = sys_configFile_readStr("IP2");
        if(!str.isEmpty()){
            ips << str;
            gBusIPMap.insert(1,str);
            ui->IP2lineEdit->setText(str);
        }

        str = sys_configFile_readStr("IP3");
        if(!str.isEmpty()){
            ips << str;
            gBusIPMap.insert(2,str);
            ui->IP3lineEdit->setText(str);
        }

        str = sys_configFile_readStr("IP4");
        if(!str.isEmpty()){
            ips << str;
            gBusIPMap.insert(3,str);
            ui->IP4lineEdit->setText(str);
        }

        sys_configFile_close();
        if(ips.size()) set_hb_IP(ips);
    }
}

/**
 * @brief IP地址验证
 * @return
 */
bool IpSettingDlg::ipCheck(int i, QLineEdit *edit)
{
    bool ret = true;
    QString str = edit->text();
    if(!str.isEmpty()) {
        ret = cm_isIPaddress(str);
        if(ret){
            gBusIPMap[i] = str;
        } else {
            edit->setText(gBusIPMap[i]);
            CriticalMsgBox box(this, tr("IP%1地址错误!").arg(i+1));
            return false;
        }
    }
    else
        ret = false;
    sys_configFile_writeParam(tr("IP%1").arg(i+1), str);

    return ret;

}


void IpSettingDlg::on_saveBtn_clicked()
{
    QStringList ips;
    QLineEdit *edit[4] = {ui->IP1lineEdit, ui->IP2lineEdit, ui->IP3lineEdit, ui->IP4lineEdit};

    for(int i=0; i<4; ++i) {
         bool ret = ipCheck(i, edit[i]);
         if(ret) ips << edit[i]->text();
    }
    set_hb_IP(ips);
}
