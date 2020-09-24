/*
 * logmaineleexportdlg.cpp
 *
 *  Created on: 2017年10月11日
 *      Author: Lzy
 */
#include "logmaineleexportdlg.h"
#include "ui_logmaineleexportdlg.h"

LogMainEleExportDlg::LogMainEleExportDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogMainEleExportDlg)
{
    ui->setupUi(this);

    mDbThread = new LogMainEleExportThread(this);
    connect(mDbThread,SIGNAL(readDbSig(QString)), this,SLOT(readDbSlot(QString)));
    connect(mDbThread,SIGNAL(exportSig(bool)), this,SLOT(exportSlot(bool)));

    mProgressWid = new ExcelExportWid(ui->widget);
    connect(mProgressWid,SIGNAL(overSig()), this,SLOT(overSlot()));
}

LogMainEleExportDlg::~LogMainEleExportDlg()
{
    delete ui;
}

void LogMainEleExportDlg::set(int id)
{
    ui->pushButton->setEnabled(false);
    mDbThread->initData(id);
}

void LogMainEleExportDlg::readDbSlot(QString str)
{
    ui->statusLab->setText(str);
}

void LogMainEleExportDlg::exportSlot(bool e)
{
    if(e)
        mProgressWid->set(gExcelExportStr->path, gExcelExportStr->file, mDbThread->mList);
    else
        overSlot();
}

void LogMainEleExportDlg:: overSlot()
{
    ui->pushButton->setEnabled(true);
}

void LogMainEleExportDlg::on_pushButton_clicked()
{
    this->close();
}
