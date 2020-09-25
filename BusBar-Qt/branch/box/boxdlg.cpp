#include "boxdlg.h"
#include "ui_boxdlg.h"

BoxDlg::BoxDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BoxDlg)
{
    ui->setupUi(this);
    com_setBackColour(tr("接插箱"), this);
//    set_background_icon(this,":/new/prefix1/image/dialog.png",QSize(815,400));
    this->setWindowFlags(Qt::WindowSystemMenuHint|Qt::WindowMinimizeButtonHint);
}

BoxDlg::~BoxDlg()
{
    delete ui;
}

void BoxDlg::initBox(int bus, int box)
{
    sDataPacket *shm = get_share_mem();
    mData = &(shm->data[bus].box[box]);

    QString name(mData->boxName);
    ui->titleLab->setText(name);
    QString version = QString("V%1.%2").arg(mData->version/10).arg(mData->version%10);
    ui->version->setText(version);

    initWid(bus, box);
}

void BoxDlg::initWid(int bus, int box)
{
    mTotalWid = new BoxTotalWid(ui->tabWidget);
    mTotalWid->initFun(bus, box);
    ui->tabWidget->addTab(mTotalWid, tr("各相信息"));

    mLineWid = new BoxLoopTableWid(ui->tabWidget);
    mLineWid->initLine(bus, box);
    ui->tabWidget->addTab(mLineWid, tr("各回路信息"));
}

void BoxDlg::on_pushButton_clicked()
{
    this->close();
    BeepThread::bulid()->beep();
}
