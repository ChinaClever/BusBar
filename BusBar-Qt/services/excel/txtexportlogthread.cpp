/*
 * txtexportlogthread.cpp
 *
 *  Created on: 2017年10月11日
 *      Author: Lzy
 */
#include "txtexportlogthread.h"

TxtExportLogThread::TxtExportLogThread(QObject *parent) : ExcelExportLogThread(parent)
{

}


/**
 * @brief 数据生成Excel
 */
void TxtExportLogThread::exportMsg(QList<QStringList> &list)
{
    bool ret = false;

    QString fileName = mFile +".txt";
    QFile data(fileName);
    if (data.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream out(&data);
        //设置文件流编码方式
        out.setCodec("GBK");

        for(int i=0; i<list.size(); ++i)
        {
            for(int j=0; j<list.at(i).size(); ++j) {
                out << list.at(i).at(j) << "\t";
            }
            out << "\n";
            mSize++;
            msleep(30);
        }
        ret = true;
        data.close();
        system("sync");
    } else {
        qDebug() << "Txt Export Log Thread Err!!!";
    }

    emit overSig(ret);
}
