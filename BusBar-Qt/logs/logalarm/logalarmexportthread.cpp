/*
 * logalarmexportthread.cpp
 *
 *  Created on: 2017年10月11日
 *      Author: Lzy
 */
#include "logalarmexportthread.h"

LogAlarmExportThread::LogAlarmExportThread(QObject *parent) : QThread(parent)
{
    mExcelStr = gExcelExportStr;
}

LogAlarmExportThread::~LogAlarmExportThread()
{
    wait();
}


void LogAlarmExportThread::initData(int busId)
{
    mEle = db_alarm_obj(busId);
    mList.clear();
    start();
}

bool LogAlarmExportThread::readDb()
{
    bool ret = true;QString msg;
    if(gLanguage == 0)msg = tr("正在读取数据，请耐心等待!");
    else msg = tr("Reading data, please be patient and wait!");
    emit readDbSig(msg);

    QStringList heads;
    if(gLanguage == 0) heads << "No." << "Date" << "Time" << tr("告警内容") << tr("详细描述");
    else heads << "No." << "Date" << "Time" << tr("Alarm content") << tr("Detailed description");
    mList << heads;

    QVector<DbAlarmItem> items = mEle->selectByDate(mExcelStr->start, mExcelStr->end);
    if(items.size() > 0)
    {
        for(int i=0; i<items.size(); ++i) {
            QStringList str;
            str << QString::number(i+1);
            str << items.at(i).date;
            str << items.at(i).time;
            str << items.at(i).type;
            str << items.at(i).msg;
            mList << str;

            if(i%15 == 0) {
                if(gLanguage == 0) msg = tr("正在解析第%1条数据，请耐心等待!").arg(i+1);
                else msg = tr("Parsing the %1 data, please be patient and wait!").arg(i+1);
                emit readDbSig(msg);
                msleep(10);
            }
        }
        if(gLanguage == 0) msg = tr("数据读取完成!!");
        else msg = tr("Data reading completed!!");
    } else {
        ret = false;
        if(gLanguage == 0) msg = tr("没有记录数据，无法导出!!");
        else msg = tr("No recorded data, unable to export!!");
    }
    emit readDbSig(msg);

    return ret;
}

void LogAlarmExportThread::run()
{
    bool ret = readDb();
    emit exportSig(ret);
}
