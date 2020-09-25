#ifndef LOGALARMWID_H
#define LOGALARMWID_H

#include <QWidget>
#include "logcom/logbtnbar.h"
#include "sqltablemodel.h"

namespace Ui {
class LogAlarmWid;
}

class LogAlarmWid : public QWidget
{
    Q_OBJECT

public:
    explicit LogAlarmWid(QWidget *parent = 0);
    ~LogAlarmWid();


protected:
    void initBtnBar();
    QString getTableName(int id);
    bool refreshTable(const QString &table);

public slots:
    void initTableSlot(int id);

protected slots:
    void initFunSLot();
    void doubleSlot(QModelIndex);
    void clearTableSlot();
    void refreshSlot();
    void exportSlot(int);

private:
    Ui::LogAlarmWid *ui;
    int mid;
    LogBtnBar *mBtnBar;
    SqlTableModel *model;
    QString m_table;
    QStringList mHeadList;
    int mCount;
};

#endif // LOGALARMWID_H
