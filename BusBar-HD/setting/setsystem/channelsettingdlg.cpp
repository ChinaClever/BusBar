#include "channelsettingdlg.h"
#include "ui_channelsettingdlg.h"


QMap<int, QString> gBusHZMap;
int gHZ= 0;
int getByCh(const QString &ch)
{
    for(int i = 0 ; i < BUS_NUM ; ++i)
    {

        if(gBusHZMap[i] == ch)
            return i;
    }
    return -1;
}

QString getChByKey(int key)
{
    return gBusHZMap[key];
}

void getHZ(int &key)
{
    gHZ = key;
}

ChannelSettingDlg::ChannelSettingDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelSettingDlg)
{
    ui->setupUi(this);

    initData();
}

ChannelSettingDlg::~ChannelSettingDlg()
{
    delete ui;
}

void ChannelSettingDlg::initData()
{
    mIndex = 0;
    bool ret = sys_configFile_open();
    if(ret)
    {
        QStringList chs;
        QString str = sys_configFile_readStr("CH1");
        if(!str.isEmpty()) {
            chs << str;
            gBusHZMap.insert(0,str);
            ui->CH1lineEdit->setText(str);
        }
        else
        {//写入默认值
            chs << CH1;
            gBusHZMap.insert(0,str);
            ui->CH1lineEdit->setText(str);
        }

        str = sys_configFile_readStr("CH2");
        chs << str;
        gBusHZMap.insert(1,str);
        ui->CH2lineEdit->setText(str);

        str = sys_configFile_readStr("CH3");
        chs << str;
        gBusHZMap.insert(2,str);
        ui->CH3lineEdit->setText(str);

        str = sys_configFile_readStr("CH4");
        chs << str;
        gBusHZMap.insert(3,str);
        ui->CH4lineEdit->setText(str);

        sys_configFile_close();
        #if ( SI_RTUWIFI == 1)
        if(chs.size()) set_ch(chs , 0);
        #endif
    }
}

/**
 * @brief CH重复验证
 * @return
 */
bool ChannelSettingDlg::repeatCheck(int index,QString str)
{
    QMap<QString, int> HZBusMap;
    for(int i = 0 ; i < BUS_NUM ; ++i)
        HZBusMap.insert(gBusHZMap[i],i);
    if(HZBusMap.contains(str) && index == getByCh(str))
        return false;
    return (HZBusMap.contains(str)) && (getByCh(str) != -1);
}

/**
 * @brief CH地址验证
 * @return
 */
bool ChannelSettingDlg::chCheck(int i, QLineEdit *edit)
{
    bool ret = true;
    QString str = edit->text();
    if(!str.isEmpty()) {
        if(str.toInt()<=525 && str.toInt()>=398&& !repeatCheck(i,str)){
            gBusHZMap[i] = str;
        } else {
            edit->setText(gBusHZMap[i]);
            CriticalMsgBox box(this, tr("CH%1应该小于398HZ或者大于525HZ , 频道重复!").arg(i+1));
            return false;
        }
    }

    sys_configFile_writeParam(tr("CH%1").arg(i+1), str);

    return ret;

}


void ChannelSettingDlg::on_saveBtn_clicked()
{
    QStringList chs;
    QLineEdit *edit[4] = {ui->CH1lineEdit, ui->CH2lineEdit, ui->CH3lineEdit, ui->CH4lineEdit};
    int recordindex = 0;
    bool ret = false;

    for(int i=0; i<4; ++i) {
         ret = chCheck(i, edit[i]);
         chs << edit[i]->text();
         if(ret) recordindex = i;
    }
    #if ( SI_RTUWIFI == 1)
    set_ch(chs , recordindex);
    #endif

    ui->CHlab->setText(tr("CH%1").arg(mIndex+1));
    ui->HZlab->setText(tr("%1HZ").arg(gHZ));
}

void ChannelSettingDlg::busChangedSlot(int index)
{
    mIndex = index;
}
