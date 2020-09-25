/*
 * logcalendardlg.cpp
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "logcalendardlg.h"
#include "ui_logcalendardlg.h"

LogCalendarDlg::LogCalendarDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogCalendarDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("日期选择"));

    connect(ui->calendarWidget,SIGNAL(selectionChanged()),this, SLOT(selectionChangedSLot()));
    selectionChangedSLot();
}

LogCalendarDlg::~LogCalendarDlg()
{
    delete ui;
}


void LogCalendarDlg::selectionChangedSLot()
{
    BeepThread::bulid()->beep();
    mDate= ui->calendarWidget->selectedDate();

    QString str = mDate.toString("yyyy-MM-dd");
    ui->timeLab->setText(str);
}


void LogCalendarDlg::on_pushButton_clicked()
{
    BeepThread::bulid()->beep();
    mDate= ui->calendarWidget->selectedDate();
      this->accept(); //接受
}
