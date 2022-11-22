#include "setlinetem.h"
#include "ui_setlinetem.h"

SetLineTem::SetLineTem(QWidget *parent, bool flag) :
    QWidget(parent),
    ui(new Ui::SetLineTem)
{
    ui->setupUi(this);
    mFlag = flag;
    indexChanged(0);
    if(mFlag){
        ui->label->setText(tr("温度1"));
        ui->label_2->setText(tr("温度2"));
        ui->label_3->setText(tr("温度3"));
    }else{
        ui->label->setText(tr("功率1"));
        ui->label_2->setText(tr("功率2"));
        ui->label_3->setText(tr("功率3"));
    }
    timer = new QTimer(this);
    timer->start(2000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

SetLineTem::~SetLineTem()
{
    delete ui;
}


void SetLineTem::updateWid()
{
    QPushButton *btn[] ={ui->temBtn_1, ui->temBtn_2, ui->temBtn_3};

    for(int i=0; i<3; ++i) {
        QString str = "";
        if(mFlag){
            str = startBoxTem->value[i] ?  QString::number(startBoxTem->value[i],10)+"℃" : "---";
            btn[i]->setText(str);
            sDataUnit *unit = &(get_share_mem()->data[mBus].box[0].env.tem);
            setBtnColor(btn[i] , unit->alarm[i] , unit->crAlarm[i]);
        }else{
            str = share_mem_get()->data[mBus].box[0].offLine ?  QString::number(startBoxPow->value[i]/COM_RATE_POW,'f', 3)+"kW" : "---";
            btn[i]->setText(str);
            sDataPowUnit *unit = &(get_share_mem()->data[mBus].box[0].data.pow);
            setBtnColor(btn[i] , unit->alarm[i] , unit->crAlarm[i]);
        }
    }
}

void SetLineTem::setBtnColor(QPushButton *label, int alarm, int crAlarm)
{
    QPalette pa;
    if(alarm) { // 告警
        pa.setColor(QPalette::ButtonText,Qt::red);
    } else  if(crAlarm) { // 预警
        pa.setColor(QPalette::ButtonText,"#CD7E80");
    } else {
        pa.setColor(QPalette::ButtonText,Qt::black);
    }
    label->setPalette(pa);
}


void SetLineTem::indexChanged(int index)
{
    mBus = index;
    if(mFlag)
        startBoxTem = &(share_mem_get()->data[index].box[0].env.tem); //获取共享内存
    else
        startBoxPow = &(share_mem_get()->data[index].box[0].data.pow); //获取共享内存
}


void SetLineTem::timeoutDone()
{
    updateWid();
}

void SetLineTem::setTem(int id)
{
    sThresholdItem item;
    item.bus = mBus;
    item.box = 0;
    item.num = id;
    if(mFlag)
        item.type = 3;
    else
        item.type = 4;

    SetThresholdDlg dlg(this);
    dlg.move(0,0);
    dlg.set(item);
    dlg.exec();
}

void SetLineTem::on_temBtn_1_clicked()
{
    setTem(0);
}

void SetLineTem::on_temBtn_2_clicked()
{
    setTem(1);
}

void SetLineTem::on_temBtn_3_clicked()
{
    setTem(2);
}
