/*
 * dbalarm.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dbalarm.h"

DbAlarm::DbAlarm()
{
    createTable();
}

void DbAlarm::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id             INTEGER primary key autoincrement not null,"
            "date           VCHAR,"
            "time           VCHAR,"
            "type           VCHAR,"
            "msg            TEXT);";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName())))
    {
        throwError(query.lastError());
    }
}

bool DbAlarm::insertItem(DbAlarmItem& item)
{
    bool ret = false;
    item.id = maxId()+1;
    QString cmd = "insert into %1 (id, date, time,type, msg) "
            "values(:id,:date,:time,:type,:msg)";
    ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}


bool DbAlarm::modifyItem(const DbAlarmItem &item, const QString &cmd)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare(cmd);
    query.bindValue(":id",item.id);
    query.bindValue(":date",item.date);
    query.bindValue(":time",item.time);
    query.bindValue(":type",item.type);
    query.bindValue(":msg",item.msg);
    ret = query.exec();
    if(!ret)
        throwError(query.lastError());
    return ret;
}

void DbAlarm::selectItem(QSqlQuery &query,DbAlarmItem &item)
{
    item.id = query.value("id").toInt();
    item.date = query.value("date").toString();
    item.time = query.value("time").toString();
    item.type = query.value("type").toString();
    item.msg = query.value("msg").toString();
}


DbAlarm* db_alarm_obj(int id)
{
    static DbAlarm* sington1  = new DbAlarm();
    static DbAlarm2* sington2  = new DbAlarm2();
    static DbAlarm3* sington3  = new DbAlarm3();
    static DbAlarm4* sington4  = new DbAlarm4();

    DbAlarm* sington = NULL;
    switch (id)
    {
    case 0:
        sington = sington1;
        break;

    case 1:
        sington = sington2;
        break;

    case 2:
        sington = sington3;
        break;

    case 3:
        sington = sington4;
        break;
    }
    return sington;
}

