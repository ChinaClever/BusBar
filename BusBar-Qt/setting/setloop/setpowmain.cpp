#include "setpowmain.h"
#include "ui_setpowmain.h"

SetPowMain::SetPowMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetPowMain)
{
    ui->setupUi(this);
    mLineTem = new SetLineTem(ui->widget_1 , false);
    mLoopPowerTem = new SetLoopPowerWid(ui->widget_2);
}

SetPowMain::~SetPowMain()
{
    delete ui;
}

void SetPowMain::updateWid(int index)
{
    mLineTem->indexChanged(index);
    mLoopPowerTem->setBus(index);
}
