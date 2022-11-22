#ifndef SETNAMESWID_H
#define SETNAMESWID_H

#include <QWidget>
#include "common.h"
#include "setshm.h"
#include <QTableWidgetItem>
#include "setnamedlg.h"

#include <QScrollBar>
#include <QPropertyAnimation>
#include <QDateTime>


namespace Ui {
class SetNamesWid;
}

class SetNamesWid : public QWidget
{
    Q_OBJECT

public:
    explicit SetNamesWid(QWidget *parent = 0);
    ~SetNamesWid();

    void indexChanged(int index);
    void updateWid();

protected:
    void initWid(int index);
    bool saveBusName();
    void initTableWidget();
    void clearWidget();
    void resetWidget();
    void checkBus();
    void setName(int row, int column);
    void setTableItem(int row, int column);
    void initScrollArea();
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_saveBtn_clicked();
    void itemDoubleClicked(QTableWidgetItem *item);
    void initFunSLot();
    void timeoutDone();
    void interfaceChangedSlot(int id);

private:
    Ui::SetNamesWid *ui;
    SetNameDlg *mSetNameDlg;

    int mIndex;
    SetShm *mSetShm;
    sBusData *mPacket ;
    QTimer* mTimer;
    QScrollBar *m_scrollBarV;
    QScrollBar *m_scrollBarH;
    bool isRun;
};

#endif // SETNAMESWID_H
