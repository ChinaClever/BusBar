#include "boxtotalwid.h"
#include "ui_boxtotalwid.h"

BoxTotalWid::BoxTotalWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoxTotalWid)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    timer->start(2000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    ui->AllWid->hide(); //隐藏和 y_MW_2018.4.23
}

BoxTotalWid::~BoxTotalWid()
{
    delete ui;
}


void BoxTotalWid::initFun(int bus, int box)
{
    sDataPacket *shm = get_share_mem();
    mBox = &(shm->data[bus].box[box]);
    mTgBox = &(shm->data[bus].box[box].tgBox);
    mLineTgBox  = &(shm->data[bus].box[box].lineTgBox);
    mEnvData = &(shm->data[bus].box[box].env);
    updateData();
}

void BoxTotalWid::timeoutDone()
{
    updateData();
    updateAll();
}

void BoxTotalWid::updateAll()
{
    QString str= "---";

    if(mBox->offLine){
        str = QString::number(mTgBox->vol) + "V";
        ui->volH->setText(str);

        str= QString::number(mTgBox->cur/COM_RATE_CUR, 'f', 1) + "A";
        ui->curH->setText(str);

        str =  QString::number(mTgBox->pow/COM_RATE_POW, 'f', 3) + "kW";
        ui->tPowH->setText(str);

        str =  QString::number(mTgBox->apPow/COM_RATE_POW, 'f', 3) + "kVA";
        ui->tApPowH->setText(str);

        str =  QString::number(mTgBox->pf/COM_RATE_PF, 'f', 2);
        ui->tPfH->setText(str);

        str =  QString::number(mTgBox->ele/COM_RATE_ELE, 'f', 1);// + "kWh";
        ui->tEleH->setText(str);

        str =  QString::number(mTgBox->tem/COM_RATE_TEM) + "°C";
        ui->temH->setText(str);
    }else{
        ui->curH->setText(str);
        ui->tPowH->setText(str);
        ui->tApPowH->setText(str);
        ui->tPfH->setText(str);
        ui->tEleH->setText(str);
        ui->volH->setText(str);
    }
}

void BoxTotalWid::updateData()
{
    if(mBox->dc){ //交流
        ui->CWid->show();
    }else{
        ui->CWid->hide();
    }

    QLabel *curLab[] = { ui->curA,  ui->curB,  ui->curC};
    QLabel *volLab[] = { ui->volA,  ui->volB,  ui->volC};
    QLabel *powLab[] = { ui->tPowA,  ui->tPowB,  ui->tPowC};
    QLabel *tApPowLab[] = { ui->tApPowA,  ui->tApPowB,  ui->tApPowC};
    QLabel *pfLab[] = { ui->tPfA,  ui->tPfB,  ui->tPfC};
    QLabel *eleLab[] = { ui->tEleA,  ui->tEleB,  ui->tEleC};
    QLabel *temLab[] = {ui->temA, ui->temB, ui->temC};
    QLabel *plLab[] = { ui->plA,  ui->plB,  ui->plC};
    QLabel *thdLab[] = {ui->thdA, ui->thdB, ui->thdC};

    for(int i=0; i<3; ++i) {
        QString str = "---";
        if(mBox->offLine){
            str = QString::number(mLineTgBox->vol[i]) + "V";
            volLab[i]->setText(str);

            str= QString::number(mLineTgBox->cur[i]/COM_RATE_CUR, 'f', 1) + "A";
            curLab[i]->setText(str);

            str= QString::number(mBox->data.pl[i]/COM_RATE_VOL, 'f', 1) ;
            plLab[i]->setText(str);

            str= QString::number(mBox->data.curThd[i]/100.0, 'f', 2) + "%";
            thdLab[i]->setText(str);

            str =  QString::number(mLineTgBox->pow[i]/COM_RATE_POW, 'f', 3) + "kW";
            powLab[i]->setText(str);

            str =  QString::number(mLineTgBox->apPow[i]/COM_RATE_POW, 'f', 3) + "kVA";
            tApPowLab[i]->setText(str);

            str =  QString::number(mLineTgBox->pf[i]/COM_RATE_PF, 'f', 2);
            pfLab[i]->setText(str);

            str =  QString::number(mLineTgBox->ele[i]/COM_RATE_ELE, 'f', 1);// + "kWh";
            eleLab[i]->setText(str);

            sDataUnit *unit = &(mEnvData->tem);
            str =  QString::number(unit->value[i]) + "°C";
            temLab[i]->setText(str);
            setLabeColor(temLab[i], unit->alarm[i], unit->crAlarm[i]);

        }else{
            volLab[i]->setText(str);
            curLab[i]->setText(str);
            plLab[i]->setText(str);
            thdLab[i]->setText(str);
            powLab[i]->setText(str);
            tApPowLab[i]->setText(str);
            pfLab[i]->setText(str);
            eleLab[i]->setText(str);
            temLab[i]->setText(str);
        }
         if(mBox->dc == 0) thdLab[i]->setText("---");
    }
}


void BoxTotalWid::setLabeColor(QLabel *label, int alarm, int crAlarm)
{
    QPalette pa;
    if(alarm) { // 告警
        pa.setColor(QPalette::WindowText,Qt::red);
    } else  if(crAlarm) { // 预警
        pa.setColor(QPalette::WindowText,"#CD7E80");
    } else {
        pa.setColor(QPalette::WindowText,Qt::black);
    }
    label->setPalette(pa);
}
