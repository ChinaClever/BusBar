#ifndef LOGBRANCHELEWID_H
#define LOGBRANCHELEWID_H

#include <QWidget>
#include "logcom/logbtnbar.h"
#include "sqltablemodel.h"

#include <QScrollBar>
#include <QPropertyAnimation>
#include <QDateTime>

namespace Ui {
class LogBranchEleWid;
}

class LogBranchEleWid : public QWidget
{
    Q_OBJECT

public:
    explicit LogBranchEleWid(QWidget *parent = 0);
    ~LogBranchEleWid();

protected:
    void initBtnBar();
    QString getTableName(int id);
    bool refreshTable(const QString &table);
    void updateColumnHidden();
    void initScrollArea();
    bool eventFilter(QObject *obj, QEvent *event);


public slots:
    void initTableSlot(int id);

protected slots:
    void initFunSLot();
    void doubleSlot(QModelIndex);
    void clearTableSlot();
    void refreshSlot();
    void exportSlot(int);

private:
     Ui::LogBranchEleWid *ui;
    int mid;
    LogBtnBar *mBtnBar;
    SqlTableModel *model;
    QString m_table;
    QStringList mHeadList;
    int mCount;
    QScrollBar *m_scrollBarV;
};

#endif // LOGBRANCHELEWID_H
