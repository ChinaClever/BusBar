#include "dbdevname.h"

DbDevName::DbDevName()
{
    createTable();
}

void DbDevName::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id             INTEGER primary key autoincrement not null,"
            "bus            INTEGER,"
            "type           INTEGER,"
            "num          INTEGER,"
            "name          TEXT);";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName())))
    {
        throwError(query.lastError());
    }
}


DbDevName *DbDevName::bulid()
{
    static DbDevName* sington = NULL;
    if(sington == NULL)
        sington = new DbDevName();
    return sington;
}

bool DbDevName::insertItem(DbNameItem& item)
{
    bool ret = false;
    item.id = maxId()+1;
    QString cmd = "insert into %1 (id, bus, type, num, name) "
                  "values(:id,:bus,:type,:num,:name)";
    ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}


bool DbDevName::updateItem(const DbNameItem &item)
{
    bool ret = false;
    QString cmd = "update %1 set "
                  "bus       = :bus,"
                  "type      = :type,"
                  "num       = :num,"
                  "name      = :name"
                  " where id   = :id";
    ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DbDevName::modifyItem(const DbNameItem &item, const QString &cmd)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare(cmd);
    query.bindValue(":id",item.id);
    query.bindValue(":bus",item.bus);
    query.bindValue(":type",item.type);
    query.bindValue(":num",item.num);
    query.bindValue(":name",item.name);
    ret = query.exec();
    if(!ret)
        throwError(query.lastError());
    return ret;
}

void DbDevName::selectItem(QSqlQuery &query,DbNameItem &item)
{
    item.id = query.value("id").toInt();
    item.bus = query.value("bus").toInt();
    item.type = query.value("type").toInt();
    item.num = query.value("num").toInt();
    item.name = query.value("name").toString();
}

int DbDevName::getId(DbNameItem &item)
{
    QString condition = QString("where bus=%1 and type=%2 and num=%3").arg(item.bus).arg(item.type).arg(item.num);
    return  maxId("id", condition);
}


bool DbDevName::saveItem(DbNameItem &item)
{
    bool ret=false;
    int id = getId(item);
    if(id > 0) {
        item.id = id;
        ret = updateItem(item);
    } else
        ret = insertItem(item);
    return ret;
}

bool DbDevName::setName(int busId, int typeId, int num, const QString &name)
{
    DbNameItem item;
    item.bus = busId;
    item.type = typeId;
    item.num = num;
    item.name = name;

    return saveItem(item);
}

QString DbDevName::getName(int busId, int typeId, int num)
{
    DbNameItem item;
    item.bus = busId;
    item.type = typeId;
    item.num = num;

    int id = getId(item);
    if(id > 0) {
        item.name = findById(id).name;
    }

    return item.name;
}
