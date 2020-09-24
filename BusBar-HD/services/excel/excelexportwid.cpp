/*
 * excelexportwid.cpp
 *
 *  Created on: 2017年10月11日
 *      Author: Lzy
 */
#include "excelexportwid.h"
#include "ui_excelexportwid.h"

ExcelExportWid::ExcelExportWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExcelExportWid)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("日志导出"));

    mExportThread = new ExcelExportLogThread(this);
    connect(mExportThread, SIGNAL(progressSig(int)), this,SLOT(progressSlot(int)));
    connect(mExportThread, SIGNAL(overSig(bool)), this,SLOT(overSlot(bool)));

    mtxtThread = new TxtExportLogThread(this);
    connect(mtxtThread, SIGNAL(progressSig(int)), this,SLOT(progressSlot(int)));
    connect(mtxtThread, SIGNAL(overSig(bool)), this,SLOT(overSlot(bool)));
}

ExcelExportWid::~ExcelExportWid()
{
    delete ui;
}


void ExcelExportWid::set(const QString &path, const QString &file, const QList<QStringList> &list, int mode)
{
    QString fileName = path + file;

    switch (mode) {
    case 1:
        mExportThread->set(fileName, list);
        break;

    case 2:
        mtxtThread->set(fileName, list);
        break;

    default:
        break;
    }
}

void ExcelExportWid::progressSlot(int progress)
{
    ui->progressBar->setValue(progress);
}

void ExcelExportWid::overSlot(bool over)
{
    int progress = 100;
    QString str =  tr("\n导出完成!!\n");

    if(!over) {
        progress = 0;
        str =  tr("\n导出失败!!\n");
    }

    ui->progressBar->setValue(progress);
    InfoMsgBox box(this,str);

    emit overSig();
}
