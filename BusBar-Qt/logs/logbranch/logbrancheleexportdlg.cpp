/*
 * logbrancheleexportdlg.cpp
 *
 *  Created on: 2017年10月11日
 *      Author: Lzy
 */
#include "logbrancheleexportdlg.h"
#include "ui_logbrancheleexportdlg.h"

LogBranchEleExportDlg::LogBranchEleExportDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogBranchEleExportDlg)
{
    ui->setupUi(this);
    initLanguage();

    mDbThread = new LogBranchEleExportThread(this);
    connect(mDbThread,SIGNAL(readDbSig(QString)), this,SLOT(readDbSlot(QString)));
    connect(mDbThread,SIGNAL(exportSig(bool)), this,SLOT(exportSlot(bool)));

    mProgressWid = new ExcelExportWid(ui->widget);
    connect(mProgressWid,SIGNAL(overSig()), this,SLOT(overSlot()));
}

LogBranchEleExportDlg::~LogBranchEleExportDlg()
{
    delete ui;
}

void LogBranchEleExportDlg::initLanguage()
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
void LogBranchEleExportDlg::set(int id)
{
    ui->pushButton->setEnabled(false);
    mDbThread->initData(id);
}

void LogBranchEleExportDlg::readDbSlot(QString str)
{
    ui->statusLab->setText(str);
}

void LogBranchEleExportDlg::exportSlot(bool e)
{
    if(e)
        mProgressWid->set(gExcelExportStr->path, gExcelExportStr->file, mDbThread->mList);
    else
        overSlot();
}

void LogBranchEleExportDlg:: overSlot()
{
    ui->pushButton->setEnabled(true);
}

void LogBranchEleExportDlg::on_pushButton_clicked()
{
    this->close();
}
