#include "setlineitem.h"
#include "ui_setlineitem.h"

SetLineItem::SetLineItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetLineItem)
{
    ui->setupUi(this);
    mBus = mLine = 0;
    mPacket = get_share_mem();

    timer = new QTimer(this);
    timer->start(2000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    connect(ui->curBar,SIGNAL(clicked()),this,SLOT(curBarClicked()));
    connect(ui->volBar,SIGNAL(clicked()),this,SLOT(volBarClicked()));
}

SetLineItem::~SetLineItem()
{
    delete ui;
}

void SetLineItem::timeoutDone()
{
    updateWidget(mBus, mLine);
}


/**
 * @brief 主路源变化，刷新界面
 * @param index
 */
void SetLineItem::updateWidget(int bus, int line)
{
    QString str = "L ";
    sBusData *busData = &(mPacket->data[bus]);
    int dc = busData->box[0].dc;
    if(!dc){
        int len = busData->box[0].rate ? busData->box[0].rate : 1;
        if(line>len) this->hide();
        str = "D ";
    }else{  //交流
        this->show();
    }

    sObjData  *objData = &(busData->box[0].data);
    ui->curLab->setText(QString::number(objData ->cur.value[line]/COM_RATE_CUR,'f', 1)+"A");
    ui->volLab->setText(QString::number(objData ->vol.value[line]/COM_RATE_VOL,'f', 0)+"V");
    ui->nameLab->setText(str+ QString::number(mLine+1));

    setProgressbarValue(ui->curBar,&(objData->cur),line);
    setProgressbarValue(ui->volBar,&(objData->vol),line);
}


void SetLineItem::setProgressbarValue(QProgressBar *bar, sDataUnit *data, int index)
{
    int max = data->max[index];
    if(max > 0)
    {
        double value = data->value[index]*1.0;
        int ret = (value/max)*100;
        bar->setValue(ret);
    }else
        bar->setValue(0);

    int cirAlarm = data->crAlarm[index];
    int alarm = data->alarm[index];
    if(alarm == 1)
        setProcessBarColor(bar,"red"); //告警
    else if(cirAlarm == 1)
        setProcessBarColor(bar,"yellow"); //预警
    else
        setProcessBarColor(bar,"green"); //正常

}

void SetLineItem::setProcessBarColor(QProgressBar *bar, QString color)
{

    QString str = "QProgressBar{border:1px solid #000000;height:20;background:transparent;text-align:center;color:rgb(134,68,54);border-radius:10px;}";
    str += QString(" QProgressBar::chunk{border-radius:10px;border:1px solid black;background-color:%1;margin:0.5px;}").arg(color);
    bar->setStyleSheet(str);
}

void SetLineItem::curBarClicked()
{
    BeepThread::bulid()->beep();

    sThresholdItem item;
    item.bus = mBus;
    item.box = 0;
    item.num = mLine;
    item.type = 2;

    SetThresholdDlg dlg(this);
    dlg.set(item);
    dlg.exec();
}

void SetLineItem::volBarClicked()
{
    BeepThread::bulid()->beep();

    sThresholdItem item;
    item.bus = mBus;
    item.box = 0;
    item.num = mLine;
    item.type = 1;

    SetThresholdDlg dlg(this);
    dlg.set(item);
    dlg.exec();
}
