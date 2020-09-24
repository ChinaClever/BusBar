#ifndef DBBRANCHELE_H
#define DBBRANCHELE_H

#include "basicsql.h"

#define DB_TABLE_BRANCHELE "branch_ele"

struct DbBranchEleItem : public DbBasicItem{
    QString name;
    double loops[10], loop;
};


class DbBranchEle : public SqlBasic<DbBranchEleItem>
{
public:
    DbBranchEle();

    QString tableName(){return QString("%1_%2").arg(DB_TABLE_BRANCHELE).arg(1) ;}
    bool insertItem(DbBranchEleItem &item); // 插入

    void createTable();
protected:
    bool modifyItem(const DbBranchEleItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,DbBranchEleItem &item);
};


class DbBranchEle2 : public DbBranchEle
{
public:
    DbBranchEle2(){ createTable(); }
    QString tableName(){return QString("%1_%2").arg(DB_TABLE_BRANCHELE).arg(2) ;}
};


class DbBranchEle3 : public DbBranchEle
{
public:
    DbBranchEle3(){ createTable(); }
    QString tableName(){return QString("%1_%2").arg(DB_TABLE_BRANCHELE).arg(3) ;}
};

class DbBranchEle4 : public DbBranchEle
{
public:
    DbBranchEle4(){ createTable(); }
    QString tableName(){return QString("%1_%2").arg(DB_TABLE_BRANCHELE).arg(4) ;}
};

DbBranchEle* db_branchEle_obj(int id);
#endif // DBBRANCHELE_H
