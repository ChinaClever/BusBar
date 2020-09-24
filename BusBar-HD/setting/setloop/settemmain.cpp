#include "settemmain.h"
#include "ui_settemmain.h"

SetTemMain::SetTemMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetTemMain)
{
    ui->setupUi(this);
    mLineTem = new SetLineTem(ui->widget_1);
    mLoopTem = new SetTempWid(ui->widget_2);
}

SetTemMain::~SetTemMain()
{
    delete ui;
}

void SetTemMain::updateWid(int index)
{
    mLineTem->indexChanged(index);
    mLoopTem->setBus(index);
}
