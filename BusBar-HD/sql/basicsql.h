#ifndef BASICSQL_H
#define BASICSQL_H

#include <QtSql>
#include <QObject>
#include "common.h"

struct DbBasicItem{
    DbBasicItem():id(-1){
        QDateTime dateTime = QDateTime::currentDateTime();
        date = dateTime.toString("yyyy-MM-dd");
        time = dateTime.toString("hh:mm:ss");
    }

    int id;
    QString date, time;
};

class BasicSql : public QObject
{
    Q_OBJECT
public:
    explicit BasicSql(QObject *parent = 0);

    enum{Remove,Insert,Update};
    virtual QString tableName() = 0;

    int  maxId(const QString &idName = "id");
    void remove(const QString &condition);

    int  maxId(const QString &idName, const QString &condition);
    int count(const QString &column_name, const QString &condition);

    QStringList listColumn(const QString &column_name, const QString &condition);
    QVector<int> listColumnToInt(const QString &column_name, const QString &condition);
    QVector<int> selectIds(const QString &condition);

    bool updateColumn(const QString& column_name, double value, const QString &condition);
    bool updateColumn(const QString& column_name, const QString& value, const QString &condition);

    bool clear(); // 清空 表
    virtual void createTable()=0;

signals:
    void itemChanged(int id,int type);
protected:
    void throwError(const QSqlError& err); /// inline static
    QString tableMarking();
    void setTableMarking(const QString& marking);
private:
    static void initDb();
    
};


//该类为sql通用类型。
template <typename T>
class SqlBasic:public BasicSql
{
public:
    void removeById(int id) {
        remove(QString("id = %1").arg(id));
        emit itemChanged(id,Remove);
    }

    void removeItem(const T& item) {
        removeById(item.id);
    }
    QVector<T> allItems() {
        return selectItems("");
    }
    QVector<T> findItemById(int id){
        return selectItems(QString("where id = %1").arg(id));
    }

    T findById(int id){
        return findItemById(id).first();
    }

    QVector<T> selectByDate(const QString &start, const QString &end) {
        QString cmd = QString("where date Between \'%2\' and  \'%3\'").arg(start).arg(end);
        return selectItems(cmd);
    }

    QVector<T> selectByDate(const QString &date) {
        return selectItems(QString("where date = %1").arg(date));
    }

protected:
    virtual void selectItem(QSqlQuery &query,T &item)=0;
    QVector<T> selectItems(const QString &condition)
    {
        QVector<T> items;
        QSqlQuery query;
        QString sql = QString("SELECT * from %1 %2").arg(tableName()).arg(condition);
        query.prepare(sql);
        if(query.exec()){
            T item;
            while(query.next()){
                selectItem(query,item);
                items.append(item);
            }
        } else {
            qDebug()<<" "<<query.lastError().databaseText();
            qDebug()<<" "<<query.lastError().driverText();
            qDebug()<<sql;
            throwError(query.lastError());
        }
        return items;
    }
};

#endif // BASICSQL_H
