#include "netsettingdlg.h"
#include "ui_netsettingdlg.h"

NetSettingDlg::NetSettingDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetSettingDlg)
{
    ui->setupUi(this);

    initData();
}

NetSettingDlg::~NetSettingDlg()
{
    delete ui;
}

void NetSettingDlg::on_pushButton_4_clicked()
{
//    this->close();
}

void NetSettingDlg::initData()
{
    QString ipStr,netmaskStr,gatewayStr,dnstr;
    foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        QList<QNetworkAddressEntry> entryList = netInterface.addressEntries();
        //遍历每一个IP地址(每个包含一个IP地址，一个子网掩码和一个广播地址)
        foreach(QNetworkAddressEntry entry, entryList)
        {
            //IP地址
            ipStr = entry.ip().toString();
            //子网掩码
            netmaskStr = entry.netmask().toString();
        }
    }

    QStringList list = ipStr.split('.');//把次IP地址,按'.'分割
    list.removeLast();//去掉最后一个
    list.append("1");//再在最后添加一个1
    gatewayStr = list.join('.');//把容器类转化为String类,存放到b
    qDebug() << gatewayStr;

    ui->lineEdit_ip->setText(ipStr);
    ui->lineEdit_mask->setText(netmaskStr);
    ui->lineEdit_gate->setText(gatewayStr);
}
