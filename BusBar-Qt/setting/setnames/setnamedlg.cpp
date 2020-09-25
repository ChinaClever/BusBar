#include "setnamedlg.h"
#include "ui_setnamedlg.h"
#include "common.h"
#include "setshm.h"
#include "setthreshold/setnetcmd.h"

extern int net_data_packets(net_dev_data *pkt, uchar *buf);

SetNameDlg::SetNameDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetNameDlg)
{
    ui->setupUi(this);
    com_setBackColour(tr("设置界面"), this);
}

SetNameDlg::~SetNameDlg()
{
    delete ui;
}

void SetNameDlg::init(int bus, int box, int loop, const QString &name)
{
    mBusId = bus;
    mBox = box;
    mLoop = loop;
    ui->nameLab->setText(name);
}

bool SetNameDlg::saveToDev()
{
    uchar array[64] = {0};
    net_dev_data pkt;
    pkt.num = mBusId; // 母线
    pkt.addr = mBox; // 插接箱
    pkt.fn[0] = 6;
    pkt.fn[1] = mLoop-1; // 相

    QString name = ui->nameEdit->text();
    pkt.len = name.size();
    pkt.data =(uchar*) name.toLocal8Bit().data();
    int len = net_data_packets(&pkt, array);

    return SetNetCmd::bulid()->sentNetData(mBusId, array, len);
}

bool SetNameDlg::save()
{
    int type=2, num = mBox;
    SetShm shm;

    DbNameItem item;
    item.bus = mBusId;
    if(mLoop) {
        type = 3;
        num = (mBox-1)*LINE_NUM + mLoop-1 ;
        saveToDev();//IP-BUSBAR设置loop名称
    }

    item.type = type;
    item.num = num;
    item.name = ui->nameEdit->text();
    shm.setName(item);
    return true;
}

void SetNameDlg::on_saveBtn_clicked()
{
    QString str = ui->nameEdit->text();
    if(!str.isEmpty()) {
        if(save())
        {
            BeepThread::bulid()->beep();
            close();
        }
    } else {
        CriticalMsgBox box(this, tr("名称不能为空!!"));
    }
}

void SetNameDlg::on_cancelBtn_clicked()
{
    BeepThread::bulid()->beep();
    close();
}
