#ifndef BOXLOOPTABLEWID_H
#define BOXLOOPTABLEWID_H

#include <QWidget>
#include "common.h"
#include <QScrollBar>
#include <QPropertyAnimation>
#include <QDateTime>

namespace Ui {
class BoxLoopTableWid;
}

class BoxLoopTableWid : public QWidget
{
    Q_OBJECT

public:
    explicit BoxLoopTableWid(QWidget *parent = 0);
    ~BoxLoopTableWid();

     void initLine(int bus, int box);

protected:
    void initTableWidget();
    void addItemContent(int row, int column, const QString &content);
    void addRowContent(QStringList &list);

    void initTable();
    void initTableWid();
    void clearTable();
    bool checkTable();

    void setTableItem(int id, int column, const QString &str);
    void setLoop(int id, int column);
    void setName(int id, int column);

    void setAlarmStatus(int id, int column, int alarm, int crAlarm);
    void setCur(int id, int column);
    void setVol(int id, int column);
    void setSw(int id, int column);
    void setPow(int id, int column);
    void setPf(int id, int column);
    void setEle(int id, int column);
    void setTemp(int id, int column);
    void updateData();
    void initScrollArea();
    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void timeoutDone();
    void initFunSLot();

private:
    Ui::BoxLoopTableWid *ui;
    QTimer *timer;

    sBoxData *mBoxData;
    sObjData *mData;
    sEnvData *mEnvData;
    char mDc;
    QScrollBar *m_scrollBarV;
    QScrollBar *m_scrollBarH;
};

#endif // BOXLOOPTABLEWID_H
