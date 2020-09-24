/*
 * logexportmodeldlg.cpp
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "logexportmodeldlg.h"
#include "ui_logexportmodeldlg.h"
#include "logcalendardlg.h"
#include "common.h"
#include <QFileDialog>

sExcelExportStr *gExcelExportStr = new sExcelExportStr();


LogExportModelDlg::LogExportModelDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogExportModelDlg)
{
    ui->setupUi(this);

    ui->startDateEdit->setDate(QDate::currentDate());
    ui->endDateEdit->setDate(QDate::currentDate());

    // 暂时不实现日表、月报表功能
    ui->groupBox_2->setHidden(true);
    this->setMaximumWidth(400);
}

LogExportModelDlg::~LogExportModelDlg()
{
    delete ui;
}

void LogExportModelDlg::on_startDateBtn_clicked()
{
    BeepThread::bulid()->beep();
    LogCalendarDlg dlg(this);
    int ret = dlg.exec();
    if(ret == QDialog::Accepted) {
        ui->startDateEdit->setDate(dlg.getDate());
    }
}

void LogExportModelDlg::on_endDateBtn_clicked()
{
    BeepThread::bulid()->beep();
    LogCalendarDlg dlg(this);
    int ret = dlg.exec();
    if(ret == QDialog::Accepted) {
        ui->endDateEdit->setDate(dlg.getDate());
    }
}

/**
 * @brief 检查输入
 */
bool LogExportModelDlg::checkInput()
{
    QString str = ui->pathEdit->text();
    if(str.isEmpty()) {
        CriticalMsgBox box(this, tr("导出路径不能为空！"));
        return false;
    }

    str = ui->fileEdit->text();
    if(str.isEmpty()) {
        CriticalMsgBox box(this, tr("导出文件名不能为空！"));
        return false;
    }

    str = ui->pathEdit->text() + ui->fileEdit->text() +".xls";
    QFile file(str);
    if (file.exists()){
        CriticalMsgBox box(this, str + tr("\n文件已存在！!"));
        return false;
    }

    str = ui->pathEdit->text() + ui->fileEdit->text() +".txt";
    QFile file1(str);
    if (file1.exists()){
        CriticalMsgBox box(this, str + tr("\n文件已存在！!"));
        return false;
    }

    QDate startDate = ui->startDateEdit->date();
    QDate endDate = ui->endDateEdit->date();
    if(startDate > endDate) {
        CriticalMsgBox box(this, tr("开始日期应早于结束日期，请重新输入日期！"));
        return false;
    }

    gExcelExportStr->path = ui->pathEdit->text();
    gExcelExportStr->file = ui->fileEdit->text();
    gExcelExportStr->start = ui->startDateEdit->date().toString("yyyy-MM-dd");
    gExcelExportStr->end = ui->endDateEdit->date().toString("yyyy-MM-dd");
    this->accept();

    return true;
}

void LogExportModelDlg::on_exportBtn_clicked()
{
    BeepThread::bulid()->beep();
    bool ret = checkInput();
    if(ret) {
        emit exportSig(0);
    }
}

void LogExportModelDlg::on_dayBtn_clicked()
{
    bool ret = checkInput();
    if(ret) {
        emit exportSig(1);
    }
}

void LogExportModelDlg::on_monthBtn_clicked()
{
    bool ret = checkInput();
    if(ret) {
        emit exportSig(2);
    }
}

void LogExportModelDlg::on_yearBtn_clicked()
{
    bool ret = checkInput();
    if(ret) {
        emit exportSig(3);
    }
}

void LogExportModelDlg::on_quitBtn_clicked()
{
    BeepThread::bulid()->beep();
    this->close();
}

void LogExportModelDlg::on_pushButton_clicked()
{
    BeepThread::bulid()->beep();
    QFileDialog dlg(this,tr("路径选择"));
    dlg.setFileMode(QFileDialog::DirectoryOnly);
    dlg.setDirectory("/mnt/sda1/");
    if(dlg.exec() == QDialog::Accepted) {
        QStringList fileNames = dlg.selectedFiles();
        ui->pathEdit->setText(fileNames.at(0) + "/");
    }
}
