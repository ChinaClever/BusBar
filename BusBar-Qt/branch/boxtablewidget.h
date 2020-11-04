#ifndef BOXTABLEWIDGET_H
#define BOXTABLEWIDGET_H

#include <QWidget>
#include "common.h"
#include <QTableWidgetItem>

#include <QScrollBar>
#include <QPropertyAnimation>
#include <QDateTime>

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
    void initScrollArea();
    bool eventFilter(QObject *obj, QEvent *event);

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
    QScrollBar *m_scrollBarV;
};

#endif // BOXTABLEWIDGET_H
