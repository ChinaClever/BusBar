#include "setthresholddlg.h"
#include "ui_setthresholddlg.h"
#include "setthread.h"

SetThresholdDlg::SetThresholdDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetThresholdDlg)
{
    ui->setupUi(this);
    com_setBackColour(tr("阈值设置"),this);
}

SetThresholdDlg::~SetThresholdDlg()
{
    delete ui;
}

void SetThresholdDlg::initSpinBox(sThresholdItem &item)
{
    ui->minBox->setValue(item.min);
    ui->maxBox->setValue(item.max);
    int range = 63;
    QString str = "A";
    switch (item.type) {
    case 1:  str = "V"; range = 400;  break;
    case 2: if(!item.box) range = 650; str = "A";  break;
    case 3: str = "℃"; range = 99;  break;
    case 4:
        {
            str = "kW"; range = 130;
            ui->mindoubleSpinBox->setValue(item.min/COM_RATE_POW);
            ui->maxdoubleSpinBox->setValue(item.max/COM_RATE_POW);
            ui->mindoubleSpinBox->setSuffix(str);
            ui->maxdoubleSpinBox->setSuffix(str);

            ui->mindoubleSpinBox->setMaximum(range);
            ui->maxdoubleSpinBox->setMaximum(range);
        }
        break;
    case 5: str = "Hz"; range = 60; break;
    }

    ui->minBox->setSuffix(str);
    ui->maxBox->setSuffix(str);

    ui->minBox->setMaximum(range);
    ui->maxBox->setMaximum(range);
}

void SetThresholdDlg::setTitle(sThresholdItem &item)
{
    QString str;
    switch (item.type) {
    case 1: str = tr("电压"); break;
    case 2: str = tr("电流"); break;
    case 3: str = tr("温度"); break;
    case 4: str = tr("功率"); break;
    case 5: str = tr("频率"); break;
    }

    sBoxData *dev = &(share_mem_get()->data[item.bus].box[item.box]); //获取共享内存
    QString nameStr = QString("插接箱%1").arg(dev->boxName);
    if(item.box == 0) nameStr = "始端箱";;

    QString busName = share_mem_get()->data[item.bus].busName;
    QString title = tr("母线%1 %2 输入%3 %4设置").arg(busName).arg(nameStr).arg(item.num+1).arg(str);
    if( item.type == 5 ) title = tr("母线%1 %2 %3设置").arg(busName).arg(nameStr).arg(str);
    ui->titleLab->setText(title);
}

void SetThresholdDlg::set(sThresholdItem &item)
{
    int rate = 1;//int rate = 1;
    sBusData *busData = &(share_mem_get()->data[item.bus]);
    sObjData *obj = &(busData->box[item.box].data);

    sDataUnit  *unit = NULL;
    sDataPowUnit  *unitPower = NULL;
    if( item.type <= 3)
        unit = &(busData->box[item.box].env.tem);
    else if(item.type == 4)
        unitPower = &(obj->pow);
    switch (item.type) {
    case 1: unit = &(obj->vol); rate = 10; break;
    case 2: unit = &(obj->cur); rate = 100; break;//rate = 10; break;
    case 3: unit = &(busData->box[item.box].env.tem);break;
    case 4: unitPower = &(obj->pow);break;
    }
    if( item.type <= 3){
        ui->mindoubleSpinBox->hide();
        ui->maxdoubleSpinBox->hide();
        item.min = unit->min[item.num] / rate;
        item.max = unit->max[item.num] / rate;
    }else if( item.type == 4 ){
        ui->minBox->hide();
        ui->maxBox->hide();
        item.min = unitPower->min[item.num] / rate;
        item.max = unitPower->max[item.num] / rate;
    }else{
        ui->mindoubleSpinBox->hide();
        ui->maxdoubleSpinBox->hide();
        item.min = busData->box[item.box].minRate;
        item.max = busData->box[item.box].maxRate;
    }

    mItem = item;
    setTitle(item);
    initSpinBox(item);
}


bool SetThresholdDlg::checkData()
{
    bool ret = true;
    int min = 0;
    int max = 0;
    if(mItem.type != 4){
        min = ui->minBox->value();
        max = ui->maxBox->value();
    }else{
        min = ui->mindoubleSpinBox->value()*COM_RATE_POW;
        max = ui->maxdoubleSpinBox->value()*COM_RATE_POW;
    }
    if(min < max)  {
        mItem.min = min;
        mItem.max = max;
    } else {
        QMessageBox::warning(this,tr("waring"),tr("最小值大于最大值！"),tr("OK"));
        ret = false;
    }

    return ret;
}


void SetThresholdDlg::on_saveBtn_clicked()
{
    bool ret = checkData();
    if(ret) {
        if(ui->checkBox->isChecked()) {
            if(mItem.box) mItem.box = 0xff;
            else mItem.bus = 0xff;
            SetThread::bulid()->append(mItem);//统一设置发两遍
        }
        SetThread::bulid()->append(mItem);
    }
    this->close();
}

void SetThresholdDlg::on_cancelBtn_clicked()
{
    this->close();
}
