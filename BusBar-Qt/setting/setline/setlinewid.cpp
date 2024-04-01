#include "setlinewid.h"
#include "ui_setlinewid.h"

SetLineWid::SetLineWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetLineWid)
{
    ui->setupUi(this);
    if(gLanguage == 0){ui->label_4->setText("编号");ui->label_5->setText("输入");
        ui->label_6->setText("类型");ui->label_7->setText("当前值");ui->label_8->setText("设置");}
    else{ui->label_4->setText("NO.");ui->label_5->setText("Input");
        ui->label_6->setText("Type");ui->label_7->setText("Current value");ui->label_8->setText("Settings");}

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
