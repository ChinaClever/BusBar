#ifndef LOGMAINELEWID_H
#define LOGMAINELEWID_H

#include <QWidget>
#include "logcom/logbtnbar.h"
#include "sqltablemodel.h"

namespace Ui {
class LogMainEleWid;
}

class LogMainEleWid : public QWidget
{
    Q_OBJECT

public:
    explicit LogMainEleWid(QWidget *parent = 0);
    ~LogMainEleWid();

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
    Ui::LogMainEleWid *ui;
    int mid;
    LogBtnBar *mBtnBar;
    SqlTableModel *model;
    QString m_table;
    QStringList mHeadList;
    int mCount;
};

#endif // LOGMAINELEWID_H
