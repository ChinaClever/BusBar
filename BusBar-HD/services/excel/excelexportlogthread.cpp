/*
 * excelexportlogthread.cpp
 *
 *  Created on: 2017年10月11日
 *      Author: Lzy
 */
#include "excelexportlogthread.h"
#include "basicExcel/BasicExcel.hpp"
using namespace YExcel;

ExcelExportLogThread::ExcelExportLogThread(QObject *parent) : QThread(parent)
{

}


ExcelExportLogThread::~ExcelExportLogThread()
{
    wait();
}

/**
 * @brief 百分制来计算
 */
int ExcelExportLogThread::getProgress()
{
    int ret = 100;

    if(mSize <= mAllSize)
        ret = (mSize *100.0) / mAllSize ;

    return ret;
}

void ExcelExportLogThread::progressSlot()
{
    int ret = getProgress();
    if(ret < 100)
         QTimer::singleShot(500,this,SLOT(progressSlot()));

    emit progressSig(ret);
}


void ExcelExportLogThread::set(const QString &file, const QList<QStringList> &list)
{
    mFile = file;
    mList = list;
    progressSlot();

    start();
}


/**
 * @brief 数据生成Excel
 */
void ExcelExportLogThread::exportMsg(QList<QStringList> &list)
{
    bool ret = false;
    BasicExcel e;
    e.New();
    BasicExcelWorksheet* sheet = e.GetWorksheet("Sheet1");
    if (sheet)
    {
        for(int i=0; i<list.size(); ++i)
        {
            for(int j=0; j<list.at(i).size(); ++j) {
                QByteArray ba = list.at(i).at(j).toLatin1();
                char* ch = ba.data();
                sheet->Cell(i, j)->SetString(ch);
            }
            mSize++;
            msleep(30);
        }

        QString fileName = mFile +".xls";
        QByteArray ba = fileName.toLatin1();
        char* ch = ba.data();

        ret = e.SaveAs(ch);
        if(!ret) {
            qDebug() << "Excel Export Log Save Err!!!";
        } else
            ret = true;
        e.Save();
        system("sync");

    } else {
        qDebug() << "Excel Export Log Thread Err!!!";
    }

    emit overSig(ret);
}


void ExcelExportLogThread::run()
{
    mSize = 0;
    mAllSize = mList.size();
    exportMsg(mList);
}
