/*
 * logswid.cpp
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "logswid.h"
#include "ui_logswid.h"

LogsWid::LogsWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogsWid)
{
    ui->setupUi(this);

    initWidget();
}

LogsWid::~LogsWid()
{
    delete ui;
}

void LogsWid::initWidget()
{
    mMainEleWid = new LogMainEleWid(ui->stackedWid);  //主路电量
    ui->stackedWid->addWidget(mMainEleWid);
    connect(this, SIGNAL(busChangedSig(int)), mMainEleWid, SLOT(initTableSlot(int)));

    mBranchEleWid = new LogBranchEleWid(ui->stackedWid); //支路电量
    ui->stackedWid->addWidget(mBranchEleWid);
    connect(this, SIGNAL(busChangedSig(int)), mBranchEleWid, SLOT(initTableSlot(int)));

    mAlarmWid = new LogAlarmWid(ui->stackedWid); //告警
    ui->stackedWid->addWidget(mAlarmWid);
    connect(this, SIGNAL(busChangedSig(int)), mAlarmWid, SLOT(initTableSlot(int)));
    connect(LogSignal::get(), SIGNAL(logTypeSig(int)), this,SLOT(logTypeSlot(int)));
}

void LogsWid::logTypeSlot(int type)
{
    switch (type) {
    case 0:
        ui->stackedWid->setCurrentWidget(mMainEleWid);
        break;
    case 1:
        ui->stackedWid->setCurrentWidget(mBranchEleWid);
        break;

    case 2:
        ui->stackedWid->setCurrentWidget(mAlarmWid);
        break;
    default:
        break;
    }
}

