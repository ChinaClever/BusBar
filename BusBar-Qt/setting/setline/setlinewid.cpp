#include "setlinewid.h"
#include "ui_setlinewid.h"

SetLineWid::SetLineWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetLineWid)
{
    ui->setupUi(this);

    int id = 0;
    mItem[id++] = new SetLineItem(ui->widget_1 , true);
    mItem[id++] = new SetLineItem(ui->widget_2 , true);
    mItem[id++] = new SetLineItem(ui->widget_3 , true);
    mItem[id++] = new SetLineItem(ui->widget_4 , false);

    for(int i=0; i<4; ++i)
        mItem[i]->initLine(i);
}

SetLineWid::~SetLineWid()
{
    delete ui;
}

void SetLineWid::updateWid(int index)
{
    for(int i=0; i<4; ++i)
        mItem[i]->updateBus(index);
}
