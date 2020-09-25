/*
 * logalarmeleexportdlg.cpp
 *
 *  Created on: 2017年10月11日
 *      Author: Lzy
 */
#include "logalarmeleexportdlg.h"
#include "ui_logalarmeleexportdlg.h"

LogAlarmEleExportDlg::LogAlarmEleExportDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogAlarmEleExportDlg)
{
    ui->setupUi(this);

    mDbThread = new LogAlarmExportThread(this);
    connect(mDbThread,SIGNAL(readDbSig(QString)), this,SLOT(readDbSlot(QString)));
    connect(mDbThread,SIGNAL(exportSig(bool)), this,SLOT(exportSlot(bool)));

    mProgressWid = new ExcelExportWid(ui->widget);
    connect(mProgressWid,SIGNAL(overSig()), this,SLOT(overSlot()));
}

LogAlarmEleExportDlg::~LogAlarmEleExportDlg()
{
    delete ui;
}


void LogAlarmEleExportDlg::set(int id)
{
    ui->pushButton->setEnabled(false);
    mDbThread->initData(id);
}

void LogAlarmEleExportDlg::readDbSlot(QString str)
{
    ui->statusLab->setText(str);
}

void LogAlarmEleExportDlg::exportSlot(bool e)
{
    if(e)
        mProgressWid->set(gExcelExportStr->path, gExcelExportStr->file, mDbThread->mList, 2);
    else
        overSlot();
}

void LogAlarmEleExportDlg:: overSlot()
{
    ui->pushButton->setEnabled(true);
}

void LogAlarmEleExportDlg::on_pushButton_clicked()
{
    this->close();
}
