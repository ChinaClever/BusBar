/*
 * sysconfigfile.cpp
 *  配置文件的操作接口
 *  配置参数的保存与读取
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "sysconfigfile.h"
#include <QtCore>

extern QString cm_pathOfData(const QString& name);
static QSettings *pConfigIni = NULL;

/**
 * 功 能：打开系统配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
bool sys_configFile_open(void)
{
    bool ret = true;

    QString fileName = "sysconfig.ini";
    QString strFilename = cm_pathOfData(fileName);

   // QString strFilename = QCoreApplication::applicationDirPath();
   // strFilename += "/" + fileName;

    if(!QFileInfo(strFilename).exists())
        ret = false;

    if(pConfigIni==NULL)
    {
        pConfigIni = new QSettings(strFilename, QSettings::IniFormat);
//        pConfigIni->setIniCodec(QTextCodec::codecForName("utf-8")); // gb18030
    }

    return ret;
}

/**
 * 功 能：关闭系统配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
void sys_configFile_close(void)
{
    delete pConfigIni;
    pConfigIni = NULL;
   // sync();
}

/**
 * 功 能：读字符串配置文件
 * 开发人员：Lzy     2013 - 七夕
 */
QString sys_configFile_readStr(QString strParameterName, QString strGroup)
{
    QString strParameter = "";
    strParameterName = "/" + strGroup + "/" + strParameterName;
    strParameter = pConfigIni->value(strParameterName).toString();
    return strParameter;
}

/**
 * 功 能：读整形串配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
int sys_configFile_readInt(QString strParameterName, QString strGroup)
{
    int nParameter = -1;
    strParameterName = "/" + strGroup + "/" + strParameterName;
    nParameter = pConfigIni->value(strParameterName).toInt();

    return nParameter;
}

/**
 * 功 能：读浮点形串配置文件
 * 开发人员：Lzy     2013 - 七夕
 */
double sys_configFile_readDouble(QString strParameterName, QString strGroup)
{
    double fParameter = 0;
    strParameterName = "/" + strGroup + "/" + strParameterName;
    fParameter = pConfigIni->value(strParameterName).toDouble();

    return fParameter;
}

/**
 * 功 能：参数写入配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
void sys_configFile_write(QString strParameterName, QString strParameter, QString strGroup)
{
    strParameterName = "/" + strGroup + "/" + strParameterName;
    pConfigIni->setValue(strParameterName, strParameter);
}

/**
 * 功 能：写入参数
 * 开发人员：Lzy     2016 - 七夕
 */
void sys_configFile_writeParam(QString name, QString value, QString strGroup)
{
    sys_configFile_open();
    sys_configFile_write(name, value, strGroup);
    sys_configFile_close();
}
