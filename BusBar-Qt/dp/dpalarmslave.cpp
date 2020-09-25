/*
 * dpalarmslave.cpp
 * 告警内容保存，告警信息提示线程
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dpalarmslave.h"
#include "dbalarm.h"

static QMutex mutex; // 互拆锁
static QString gEmailStr; // 告警邮件的内容
static QStringList gAlarmStr; // 实时告警内容 存储格式 母线名称，告警类型，告警内容

/**
 * @brief 获取告警邮件的内容
 * @return
 */
QString get_email_str()
{
    QMutexLocker locker(&mutex);
    QString str = gEmailStr;
    gEmailStr.clear();

    return str;
}

/**
 * @brief 获取实时告警内容
 * @return  实时告警内容 存储格式 母线名称，告警类型，告警内容
 */
QStringList get_alarm_str()
{
    QMutexLocker locker(&mutex);
    QStringList str = gAlarmStr;
    gAlarmStr.clear();

    return str;
}

int get_alarm_len()
{
    return gAlarmStr.size();
}


DpAlarmSlave::DpAlarmSlave(QObject *parent) : QThread(parent)
{
    isRun = false;
    shm = get_share_mem(); // 获取共享内存

    timer = new QTimer(this);
    timer->start(5*1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}


DpAlarmSlave::~DpAlarmSlave()
{
    isRun = false;
    wait();
}

void DpAlarmSlave::timeoutDone()
{
    if(isRun == false) {
        start();
    }
}


void DpAlarmSlave::saveMsg(const QString &typeStr, const QString &str)
{
    DbAlarmItem item;
    item.type = typeStr;
    item.msg = str;
    db_alarm_obj(mBusId)->insertItem(item);
    msleep(5);

    QMutexLocker locker(&mutex);
    gEmailStr += "\t" + typeStr + "\t" +str + "\n"; //邮件发送内容
}


void DpAlarmSlave::unitAlarm(QString &typeStr, QString &msg, sDataUnit &unit, double rate, const QString &sym)
{
    for(int i=0; i<3; ++i)
    {
        QString str=msg, tempStr = typeStr;
        if(unit.alarm[i])
        {
            tempStr = typeStr + tr("告警");
            str += tr("%1，当前值：%2%3, 最小值：%4%5, 最大值：%6%7").arg(i+1)
                    .arg(unit.value[i]/rate).arg(sym)
                    .arg(unit.min[i]/rate).arg(sym)
                    .arg(unit.max[i]/rate).arg(sym);

            if(unit.alarm[i] == 1){
                unit.alarm[i] = 2;
                saveMsg(typeStr, str);
            }
        }
        else if(unit.crAlarm[i])
        {
            tempStr = typeStr +  tr("预警");
            str += tr("%1，当前值：%2%3, 临界下限值：%4%5, 临界上限值：%6%7").arg(i+1)
                    .arg(unit.value[i]/rate).arg(sym)
                    .arg(unit.crMin[i]/rate).arg(sym)
                    .arg(unit.crMax[i]/rate).arg(sym);
        }

        // 实时告警信息
        if((unit.alarm[i]) || (unit.crAlarm[i])) {
            mAlarmStr << shm->data[mBusId].busName;
            mAlarmStr << tempStr;
            mAlarmStr << str;
        }
    }
}

void DpAlarmSlave::unitAlarmVA(sBoxData &box, QString &typeStr, QString &msg, sDataUnit &unit, double rate, const QString &sym)
{
    for(int i=0; i<LINE_NUM_MAX; ++i)
    {
        //---------------------[区分日志关键字]-------------------
        QString alarmStr = "Line";
        int id = i;
        if(box.dc){
            int divisor   =  id/3; //除数
            int remainder =  id%3;//余数
            alarmStr = QString((char)('A' + remainder))+ QString("%1").arg(divisor + 1);
        }else{
            alarmStr = "D" + QString("%1").arg(id+1);
        }
        //---------------------------------------------------

        QString str=msg, tempStr = typeStr;
        if(unit.alarm[i])
        {
            tempStr = typeStr + tr("告警");
            str += tr("%1，当前值：%2%3, 最小值：%4%5, 最大值：%6%7").arg(alarmStr)
                    .arg(unit.value[i]/rate).arg(sym)
                    .arg(unit.min[i]/rate).arg(sym)
                    .arg(unit.max[i]/rate).arg(sym);

            if(unit.alarm[i] == 1){
                unit.alarm[i] = 2;
                saveMsg(typeStr, str);
            }
        }
        else if(unit.crAlarm[i])
        {
            tempStr = typeStr +  tr("预警");
            str += tr("%1，当前值：%2%3, 临界下限值：%4%5, 临界上限值：%6%7").arg(alarmStr)
                    .arg(unit.value[i]/rate).arg(sym)
                    .arg(unit.crMin[i]/rate).arg(sym)
                    .arg(unit.crMax[i]/rate).arg(sym);
        }

        // 实时告警信息
        if((unit.alarm[i]) || (unit.crAlarm[i])) {
            mAlarmStr << shm->data[mBusId].busName;
            mAlarmStr << tempStr;
            mAlarmStr << str;
        }
    }
}


void DpAlarmSlave::boxAlarm(sBoxData &box)
{
    if(box.offLine) {
        if(box.boxAlarm)  {
            QString typeStr = tr("回路电流");
            if(box.boxCurAlarm) {
                QString msg = tr("插接箱：%1，").arg(box.boxName);
                unitAlarmVA(box, typeStr, msg, box.data.cur, COM_RATE_CUR, "A");
            }

            typeStr = tr("回路电压");
            if(box.boxVolAlarm) {
                QString msg = tr("插接箱：%1，").arg(box.boxName);
                unitAlarmVA(box, typeStr, msg, box.data.vol, COM_RATE_VOL, "V");
            }

            typeStr = tr("插接箱温度");
            if(box.boxEnvAlarm) {
                QString msg = tr("插接箱：%1，温度").arg(box.boxName);
                unitAlarm(typeStr, msg, box.env.tem, COM_RATE_TEM, "°C");
            }
        }
    } else {
        mAlarmStr << shm->data[mBusId].busName;
        mAlarmStr << tr("插接箱离线");
        mAlarmStr << tr("插接箱：%1 已离线").arg(box.boxName);
    }
}

void DpAlarmSlave::busAlarm(int id)
{
    mBusId = id;
    sBusData *bus = &(shm->data[id]);
    sBoxData *busBox = &(bus->box[0]);
    //----------------[区分日志关键字]-------------------
    QString alarmStr = "输入";
    if(!busBox->dc){ // 直流
        alarmStr = "输入";
    }
    //--------------------------------------------------

    if(busBox->boxAlarm) {
        if(busBox->boxCurAlarm) { // 总线电流告警
            QString typeStr = tr("主路电流");
            QString msg = tr("母线：%1，%2 ").arg(bus->busName).arg(alarmStr);
            unitAlarm(typeStr, msg, busBox->data.cur, COM_RATE_CUR, "A");
        }

        if(busBox->boxVolAlarm) { // 总线电压告警
            QString typeStr = tr("主路电压");
            QString msg = tr("母线：%1，%2 ").arg(bus->busName).arg(alarmStr);
            unitAlarm(typeStr, msg, busBox->data.vol, COM_RATE_VOL, "V");
        }

        if(busBox->boxEnvAlarm) { // 温度
            QString typeStr = tr("主路温度");
            QString msg = tr("母线：%1，温度").arg(bus->busName);
            unitAlarm(typeStr, msg, busBox->env.tem, COM_RATE_TEM, "°C");
        }
    }

    for(int i=1; i<=bus->boxNum; ++i) {
        boxAlarm(bus->box[i]);
    }
}

void DpAlarmSlave::checkAlarm()
{
    for(int i=0; i<BUS_NUM; ++i)
        busAlarm(i);

    QMutexLocker locker(&mutex);
    gAlarmStr = mAlarmStr;
    mAlarmStr.clear();
}

void DpAlarmSlave::run()
{
    if(isRun == false)
    {
        isRun  = true;
        checkAlarm();
        isRun  = false;
    }
}
