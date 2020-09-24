#include "homeboxwid.h"
#include "ui_homeboxwid.h"
#include "box/boxdlg.h"
#include "interfacechangesig.h"
#include "beepthread.h"

HomeBoxWid::HomeBoxWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeBoxWid)
{
    ui->setupUi(this);
    initWid();
    mBoxNum = mBusID = 0;

    isRun = true;
    timer = new QTimer(this);
    timer->start(500 + rand()%100); //防止所有对象同时申请刷新
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    connect(InterfaceChangeSig::get(), SIGNAL(typeSig(int)), this,SLOT(interfaceChangedSlot(int)));
}

HomeBoxWid::~HomeBoxWid()
{
    delete ui;
}

void HomeBoxWid::initFun(int base, int id)
{
    mID = base*10 + id+1;
    sDataPacket *shm = get_share_mem();
    mBoxNum = shm->data[mBusID].boxNum;
    mData = &(shm->data[mBusID].box[mID]);
    ui->titleLab->setText(QString::number(mID));
    updateData();
}

void HomeBoxWid::interfaceChangedSlot(int id)
{
    if(id == 1) {
        isRun = true;
    } else {
        isRun = false;
    }
}

void HomeBoxWid::busChangeSlot(int id)
{
    mBusID = id;
    sDataPacket *shm = get_share_mem();
    mData = &(shm->data[id].box[mID]);
    updateData();
}

/**
 * @brief 更新电流显示
 */
void HomeBoxWid::updateData()
{
    QPalette pe;
    QString str =  QString::number(mData->tgBox.cur/COM_RATE_CUR,'f',1) + "A";
    if(mData->offLine)   // 在线
    {
        if(mData->boxCurAlarm == 2)
            pe.setColor(QPalette::WindowText,Qt::red);
        else  if(mData->boxCurAlarm == 1)
            pe.setColor(QPalette::WindowText,"#CD7E80");
        else
            pe.setColor(QPalette::WindowText,Qt::black);
    } else {
        str = "---";
    }
    ui->curLab->setText(str);
    ui->curLab->setPalette(pe);
}

void HomeBoxWid::updateAlarmIcon(QLabel *lab,int volAlarm, int curALarm, int envALarm)
{
    QString str;
    if(volAlarm == 2 || curALarm == 2 || envALarm == 2) {
        str = "boxalarm";
    } else if(volAlarm == 1 || curALarm == 1 || envALarm == 1) {
        str = "boxciralarm";
    } else {
        str = "boxonline";
    }
    setBackgroundImage(lab,str);
}

/**
 * @brief 更新告警图片
 */
void HomeBoxWid::updateAlarmStatus()
{
    if(mData->offLine) {
        updateAlarmIcon(ui->iconLab_1,  mData->boxVolAlarm, mData->boxCurAlarm, mData->boxEnvAlarm);
        updateAlarmIcon(ui->iconLab_2,  mData->boxVolAlarm, mData->boxCurAlarm, mData->boxEnvAlarm);
        updateAlarmIcon(ui->iconLab_3,  mData->boxVolAlarm, mData->boxCurAlarm, mData->boxEnvAlarm);
    } else { // 离线
        setBackgroundImage(ui->iconLab_1, "boxoffine");
        setBackgroundImage(ui->iconLab_2, "boxoffine");
        setBackgroundImage(ui->iconLab_3, "boxoffine");
    }

    bool hidden = false;
    if(mID > mBoxNum) hidden = true;
    this->setHidden(hidden);
}

void HomeBoxWid::timeoutDone()
{
    if(isRun) {
        updateData();
        updateAlarmStatus();
    }
}

void HomeBoxWid::on_pushButton_clicked()
{
    BeepThread::bulid()->beep();
    BoxDlg dlg(0);
    dlg.initBox(mBusID, mID);
    dlg.exec();
}

void HomeBoxWid::initWid()
{
    ui->curLab->hide(); //隐藏和——by_MW_2018.04.23

    QSize size(95,95);
    set_background_icon(this, ":/new/prefix1/image/round.png", size);
}

void HomeBoxWid::setBackgroundImage(QLabel *lab, QString name)
{
    QString str = QString("border-image: url(:/new/prefix1/image/%1.png);").arg(name);
    lab->setStyleSheet(str);
}
