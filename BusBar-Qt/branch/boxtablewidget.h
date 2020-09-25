#ifndef BOXTABLEWIDGET_H
#define BOXTABLEWIDGET_H

#include <QWidget>
#include "common.h"
#include <QTableWidgetItem>

namespace Ui {
class BoxTableWidget;
}

class BoxTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BoxTableWidget(QWidget *parent = 0);
    ~BoxTableWidget();

protected:
    void initTableWidget();
    void addItemContent(int row, int column, const QString &content);
    void addRowContent(QStringList &list);

    void initTable();
    void initTableWid();
    void clearTable();
    bool checkTable();

    void setTableItem(int id, int column, const QString &str);
    void setName(int id, int column);
    void setAlarmStatus(int id, int column);
    void setCur(int id, int line, int column);
    void setEle(int id, int line, int column);
    void updateData();

public slots:
    void busChangeSlot(int id);
    void timeoutDone();
    void initFunSLot();
    void interfaceChangedSlot(int id);

protected slots:
    void getItem(QTableWidgetItem*);

private:
    Ui::BoxTableWidget *ui;

    int mBusID;
    sBusData *mBoxData;
    QTimer *timer;
    bool isRun;
};

#endif // BOXTABLEWIDGET_H
