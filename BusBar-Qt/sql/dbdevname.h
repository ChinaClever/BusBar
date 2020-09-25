#ifndef DBDEVNAME_H
#define DBDEVNAME_H

#include "basicsql.h"

#define DB_TABLE_DEVNAME "devname"

struct DbNameItem{
    int id;
    int bus; // 母线编号 0~3
    int type; // 名称类型 1 母线名称   2 插接箱名称  3 回路名称
    int box;
    int num; // 编号
    QString name;
};

class DbDevName : public SqlBasic<DbNameItem>
{
    DbDevName();
public:
    static DbDevName* bulid();
    bool saveItem(DbNameItem &item);

    bool setName(int busId, int typeId, int num, const QString &name);
    QString getName(int busId, int typeId, int num);

protected:
    void createTable();
    bool insertItem(DbNameItem &item); // 插入
    bool updateItem(const DbNameItem &item);

    bool modifyItem(const DbNameItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,DbNameItem &item);

    int getId(DbNameItem &item);
    QString tableName(){return QString("%1").arg(DB_TABLE_DEVNAME);}
};

#endif // DBDEVNAME_H
