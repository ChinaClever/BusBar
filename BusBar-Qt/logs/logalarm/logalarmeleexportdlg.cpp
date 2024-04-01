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
    initLanguage();
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

void LogAlarmEleExportDlg::initLanguage()
{
    if(gLanguage == 0){
        ui->label->setText("日志导出");
        ui->label_2->setText("导出状态");
        ui->label_4->setText("导出进度：");
        ui->pushButton->setText("退出");
    }else{
        ui->label->setText("Log export");
        ui->label_2->setText("Export Status:");
        ui->label_4->setText("Export Progress:");
        ui->pushButton->setText("Quit");
    }
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
