/*
 * common.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "common.h"
#include <QWidget>
#include <QHeaderView>
#include <time.h>
#include <linux/rtc.h>
#include <sys/time.h>
#include <unistd.h>
#include <QtGlobal>

void com_setBackColour(const QString &str,QWidget *target)
{
    target->setWindowTitle(str);
    //target->setWindowFlags(Qt::FramelessWindowHint);
    target->setAutoFillBackground(true) ;
    QPalette paletteTool = target->palette() ;
    QBrush brushToolPic(QColor(WIDGET_BACK_COLOR)) ;
    QBrush brushToolwhite(QColor(BUTTON_BACK_COLOR)) ;
    paletteTool.setBrush(QPalette::Window,brushToolPic) ;
    paletteTool.setBrush(QPalette::Base,brushToolPic) ;
    paletteTool.setBrush(QPalette::Button,brushToolwhite) ;
    target->setPalette(paletteTool) ;

    target->setStyleSheet(BTN_FOCUS_BACK_COLOR);
    target->setWindowIcon(QIcon(":/image/logo.jpg")); //
}



/***
  * 获取程序数据目录
  */
QString cm_pathOfData(const QString& name)
{
//#ifdef(Q_OS_ANDROID)
//    QDir dataDir("/root/");// = QDir::home();
//    QString dirName = ".CleverBusbar";
//    if(!dataDir.exists(dirName))
//        dataDir.mkdir(dirName);
//    dataDir.cd(dirName);
//    qDebug()<<"linux"<<dataDir<<endl;
//    return dataDir.absoluteFilePath(name);
//#endif
#ifdef Q_OS_LINUX
    QDir dataDir("/sdcard/Android/data/");// = QDir::home();
    QString dirName = "com.android.CleverBusbar";
    if(!dataDir.exists(dirName))
        dataDir.mkdir(dirName);
    dataDir.cd(dirName);
    return dataDir.absoluteFilePath(name);
#endif
}

/***
  *判断一个字符串是否为纯数字
  */
bool cm_isDigitStr(const QString &src)
{
    QByteArray ba = src.toLatin1();//QString 转换为 char*
    const char *s = ba.data();

    while(*s && *s>='0' && *s<='9') s++;

    if (*s) //不是纯数字
        return false;

    return true;
}

/**
 * @brief 是否为IP地址
 * @param ip
 * @return
 */
bool cm_isIPaddress(const QString& ip)
{
    QRegExp regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(regExp.exactMatch(ip))
        return true ;

    return false;
}

/***
  * 获取共享内存
  */
sDataPacket *get_share_mem()
{
    static sDataPacket *shm = NULL;
    if(shm == NULL) {
        shm = share_mem_get();
        share_mem_init();
        shm->initFlag = false;
    }

    return shm;
}

void set_background_icon(QWidget *widget, const QString &icon)
{
    QPalette palette;
    QPixmap map = QPixmap(icon);
    palette.setBrush(QPalette::Background, QBrush(map));
    widget->setAutoFillBackground(true);
    widget->setPalette(palette);
}

void set_background_color(QWidget *widget, const QColor &color)
{
    QPalette palette;
    palette.setColor(QPalette::Background, color);
    widget->setAutoFillBackground(true);
    widget->setPalette(palette);
}


void set_background_icon(QWidget *widget, const QString &icon,const QSize &size)
{    
    QPalette palette; //调色板
    QPixmap map = QPixmap(icon).scaled(size);
    palette.setBrush(QPalette::Background, QBrush(map)); //刷
    widget->setAutoFillBackground(true); //设置自动填充地面
    widget->setPalette(palette);
}


/**
 * @brief getPassword 读取系统密码
 * @return
 */
QString getPassword()
{
    QString passpord = "";

    bool ret = sys_configFile_open();  //打开配置文件
    if(ret)
    {
        passpord = sys_configFile_readStr("password");
        sys_configFile_close();
    }

    return passpord;
}

/**
 * @brief getBoxNum
 * @param index 即插接厢标号
 * @return 
 */
int getBoxNum(int index)
{
    int boxNum = -1 ;
    bool ret = sys_configFile_open();  //打开配置文件
    if(ret)
    {
        QString strGroup = QString("Line%1").arg(index+1);
        boxNum = sys_configFile_readInt("boxNum",strGroup);
        sys_configFile_close();
    } 
    return boxNum;
}

int getRateCur(int index)
{
    int rateCur = 0 ;
    bool ret = sys_configFile_open();  //打开配置文件
    if(ret)
    {
        QString strGroup = QString("Line%1").arg(index+1);
        rateCur = sys_configFile_readInt("rateCur",strGroup);
        sys_configFile_close();
    }  
    return rateCur;
}


