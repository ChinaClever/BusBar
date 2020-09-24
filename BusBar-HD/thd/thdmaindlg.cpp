#include "thdmaindlg.h"
#include "ui_thdmaindlg.h"

ThdMainDlg::ThdMainDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ThdMainDlg)
{
    ui->setupUi(this);

    mChartWid = new ThdChartWid(ui->chartwid);
    mTableWid = new ThdTableWid(ui->tablewid);

    timer = new QTimer(this);
    timer->start(2000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

ThdMainDlg::~ThdMainDlg()
{
    delete ui;
}

void ThdMainDlg::initBus(int id)
{
    sDataPacket *shm = get_share_mem();
    mBusData = &(shm->data[id]);

    on_volAbtn_clicked();
}


void ThdMainDlg::timeoutDone()
{
    QString str = QString::number((*mLineThd/100.0),'f', 2) + " %";
    ui->thdLab->setText(str);

    mChartWid->updateData(mThd, 30);
    mTableWid->updateData(mThd, 30);
}

void ThdMainDlg::setLine(int i, int l)
{
    if(i) {
        mThd = mBusData->thdData.curThd[l];
        mLineThd = &(mBusData->box[0].data.curThd[l]);
    } else {
        mThd = mBusData->thdData.volThd[l];
        mLineThd = &(mBusData->box[0].data.volThd[l]);
    }
    timeoutDone();
}

void ThdMainDlg::on_volAbtn_clicked()
{
    setLine(0, 0);
}

void ThdMainDlg::on_volBbtn_clicked()
{
    setLine(0, 1);
}

void ThdMainDlg::on_volCbtn_clicked()
{
    setLine(0, 2);
}

void ThdMainDlg::on_curAbtn_clicked()
{
    setLine(1, 0);
}

void ThdMainDlg::on_curBbtn_clicked()
{
     setLine(1, 1);
}

void ThdMainDlg::on_curCbtn_clicked()
{
    setLine(1, 2);
}

void ThdMainDlg::on_quitBtn_clicked()
{
    this->close();
}
