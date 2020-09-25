/*
 * logmaineleexportthread.cpp
 *
 *  Created on: 2017年10月11日
 *      Author: Lzy
 */
#include "logmaineleexportthread.h"


LogMainEleExportThread::LogMainEleExportThread(QObject *parent) : QThread(parent)
{
    mExcelStr = gExcelExportStr;
}

LogMainEleExportThread::~LogMainEleExportThread()
{
    wait();
}

void LogMainEleExportThread::initData(int busId)
{
    mEle = db_mainEle_obj(busId);
    mList.clear();
    start();
}

bool LogMainEleExportThread::readDb()
{
    bool ret = true;
    QString msg = tr("正在读取数据，请耐心等待!");
    emit readDbSig(msg);

    QStringList heads;
    heads << "No." << "Date" << "Time" << "A(kWh)" << "B(kWh)" << "C(kWh)" << "Bus(kWh)";
    mList << heads;

    QVector<DbMainEleItem> items = mEle->selectByDate(mExcelStr->start, mExcelStr->end);
    if(items.size() > 0)
    {
        for(int i=0; i<items.size(); ++i) {
            QStringList str;
            str << QString::number(i+1);
            str << items.at(i).date;
            str << items.at(i).time;
            str << QString::number(items.at(i).line_1);
            str << QString::number(items.at(i).line_2);
            str << QString::number(items.at(i).line_3);
            str << QString::number(items.at(i).line);
            mList << str;

            if(i%15 == 0) {
                msg = tr("正在解析第%1条数据，请耐心等待!").arg(i+1);
                emit readDbSig(msg);
                msleep(10);
            }
        }
        msg = tr("数据读取完成!!");
    } else {
        ret = false;
        msg = tr("没有记录数据，无法导出!!");
    }
    emit readDbSig(msg);

    return ret;
}

void LogMainEleExportThread::run()
{
    bool ret = readDb();
    emit exportSig(ret);
}
