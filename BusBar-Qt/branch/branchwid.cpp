#include "branchwid.h"
#include "ui_branchwid.h"

BranchWid::BranchWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BranchWid)
{
    ui->setupUi(this);
    mTableWid = new BoxTableWidget(ui->widget);
    connect(this, SIGNAL(busChangedSig(int)), mTableWid, SLOT(busChangeSlot(int)));
}


BranchWid::~BranchWid()
{
    delete ui;
}
