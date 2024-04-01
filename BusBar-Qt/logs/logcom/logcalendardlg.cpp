/*
 * logcalendardlg.cpp
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "logcalendardlg.h"
#include "ui_logcalendardlg.h"
#include "common.h"

LogCalendarDlg::LogCalendarDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogCalendarDlg)
{
    ui->setupUi(this);
    if(gLanguage == 0) this->setWindowTitle(tr("日期选择"));
    else this->setWindowTitle(tr("Date selection"));
    intlanguage();
    connect(ui->calendarWidget,SIGNAL(selectionChanged()),this, SLOT(selectionChangedSLot()));
    selectionChangedSLot();
}

LogCalendarDlg::~LogCalendarDlg()
{
    delete ui;
}

void LogCalendarDlg::intlanguage()
{
    if(gLanguage == 0){
        ui->label->setText("时间选择");
        ui->label_2->setText("选择时间:");
        ui->pushButton->setText("确定");
        ui->calendarWidget->setLocale(QLocale::Chinese);
    }else{
        ui->label->setText("Time selection");
        ui->label_2->setText("Selection period:");
        ui->pushButton->setText("Confirm");
        ui->calendarWidget->setLocale(QLocale::English);
    }
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
