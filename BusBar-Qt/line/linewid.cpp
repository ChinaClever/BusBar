#include "linewid.h"
#include "ui_linewid.h"
#include "interfacechangesig.h"
#include "thdmaindlg.h"

LineWid::LineWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LineWid)
{
    ui->setupUi(this);
    initLanguage();
    mIndex = 0;
    initFun();
    initWid();
    connect(this, SIGNAL(busChangedSig(int)), this, SLOT(indexChanged(int)));
    connect(InterfaceChangeSig::get(), SIGNAL(typeSig(int)), this,SLOT(interfaceChangedSlot(int)));
}

LineWid::~LineWid()
{
    delete ui;
}

void LineWid::initFun()
{
    sDataPacket *shm = get_share_mem();
    mData = &(shm->data[mIndex]);
    isRun = false;
    timer = new QTimer(this);
    timer->start(3*1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}
void LineWid::initLanguage()
{
    if(gLanguage == 0){
        ui->label_10->setText("防雷:");
        ui->label_2->setText("版本:");
        ui->label_5->setText("输入:");
        ui->label_13->setText("电压:");
        ui->label_9->setText("电流:");
        ui->label_3->setText("过载电流:");
        ui->label_8->setText("有功功率:");
        ui->label_6->setText("功率因数:");
        ui->label_4->setText("温度:");
        ui->label_7->setText("电能:");
        ui->thdBtn->setText("谐波分析:");
    }else{
        ui->label_10->setText("Lightning protection:");
        ui->label_2->setText("Version:");
        ui->label_5->setText("Input:");
        ui->label_13->setText("Voltage:");
        ui->label_9->setText("Current:");
        ui->label_3->setText("Overload current:");
        ui->label_8->setText("Active power:");
        ui->label_6->setText("Active power:");
        ui->label_4->setText("Temperature :");
        ui->label_7->setText("Electric energy:");
        ui->thdBtn->setText("Harmonic analysis:");
    }
}

void LineWid::initWid()
{
    //    mTotalWid = new LineTotalWid(ui->totalWid);
    //    connect(this, SIGNAL(busChangedSig(int)), mTotalWid, SLOT(busChangeSlot(int)));
    //    initTotalWid(); //表盘界面
    ui->totalWid->hide(); //隐藏和 By_MW_2018.4.23

    mLineTable = new LineTable(ui->widget) ;
    //三总线界面
    mLine[0] = new LineRoadWid(ui->line1Wid);
    mLine[1] = new LineRoadWid(ui->line2Wid);
    mLine[2] = new LineRoadWid(ui->line3Wid);

    for(int i=0; i<3; ++i) {
        LineRoadWid *line  = mLine[i];
        line->initLine(i);
        connect(this, SIGNAL(busChangedSig(int)), line, SLOT(busChangeSlot(int)));
    }

    ui->lpsLab->hide();
    ui->label_10->hide();
}


void LineWid::interfaceChangedSlot(int id)
{
    if(id == 2) {
        isRun = true;
    } else {
        isRun = false;
    }
}

void LineWid::timeoutDone()
{
    if(isRun) {
        QString str;
        if(mData->box[0].dc){ //交流
            str= QString::number(mData->box[0].rate) + "Hz";
            ui->rateLab->setText(str); //频率
            if(gLanguage == 0)ui->label->setText("频率：");
            else ui->label->setText("Frequency：");
            ui->thdBtn->setHidden(false);
            ui->widget->setHidden(false);
            mLineTable->updateData(mData->box[0]);
            if(gLanguage == 0)ui->lpsLab->setText(mData->box[0].lps==0?"正常":"损坏");
            else ui->lpsLab->setText(mData->box[0].lps==0?"normal":"damage");
            QPalette pa;
            if(mData->box[0].lps==1)
            {
                pa.setColor(QPalette::WindowText, Qt::red);
                ui->lpsLab->setPalette(pa);
            }
            else {
                pa.setColor(QPalette::WindowText, Qt::black);
                ui->lpsLab->setPalette(pa);
            }
        }else{
            if(gLanguage == 0){
                str= QString::number(mData->box[0].rate) + "路";
                ui->rateLab->setText(str); //频率
                ui->label->setText("输入：");
            }else{
                str= QString::number(mData->box[0].rate) + " load";
                ui->rateLab->setText(str); //频率
                ui->label->setText("Input：");
            }
            ui->thdBtn->setHidden(false);
            ui->widget->setHidden(true);
        }


        //------[版本号]------------
        QString version = QString("V%1.%2").arg(mData->box[0].version/10).arg(mData->box[0].version%10);
        ui->version->setText(version);
//        updateTotalWid();
//        updatePlot();
    }
}


void LineWid::initTotalWid()
{    
    QHBoxLayout *layout = new QHBoxLayout(ui->totalWid);

    mCurPlot = new CustomDialPlot(ui->totalWid);
    mVolPlot = new CustomDialPlot(ui->totalWid);
    mPwPlot = new CustomDialPlot(ui->totalWid);
    // mPfPlot = new CustomDialPlot(ui->totalWid);

    layout->addWidget(mVolPlot);
    layout->addWidget(mCurPlot);
    layout->addWidget(mPwPlot);
    // layout->addWidget(mPfPlot);

    mVolPlot->setUnit("V");
    mCurPlot->setUnit("A");
    mPwPlot->setUnit("kW");

    // mPfPlot->setUnit("");
    // mPfPlot->setRange(0,1);
}

void LineWid::updateTotalWid()
{
    int max = 0;
    for(int i=0; i<3; ++i) max += mData->box[0].data.cur.max[i];
    if(max<1000) max = 1000;
    mCurPlot->setRange(0,max/COM_RATE_CUR + 1);

    max *= 275;
    mPwPlot->setRange(0,max/COM_RATE_POW);
    mVolPlot->setRange(0,275);
}


void LineWid::updatePlot()
{    
    sDataPacket *shm = get_share_mem();
    sTgObjData *tgBusData = &(shm->data[mIndex].box[0].tgBox);
    mVolPlot->setValue(tgBusData->vol);        //表盘更新 V
    mCurPlot->setValue(tgBusData->cur/COM_RATE_CUR); //A
    mPwPlot->setValue(tgBusData->pow/COM_RATE_POW);  //W
    //    mPfPlot->setValue(tgBusData->pf/COM_RATE_PF);
}

void LineWid::indexChanged(int index)
{
    mIndex = index;
    //-------[未随着更新]------------ 故ADD—— By_MW
    sDataPacket *shm = get_share_mem();
    mData = &(shm->data[mIndex]);
    //-------------------------------------------
}


void LineWid::on_thdBtn_clicked()
{
    ThdMainDlg dlg(this);
    dlg.initBus(mIndex);
    dlg.exec();
}
