#ifndef SETNAMESWID_H
#define SETNAMESWID_H

#include <QWidget>
#include "common.h"
#include "setshm.h"
#include <QTableWidgetItem>
#include "setnamedlg.h"

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

private slots:
    void on_saveBtn_clicked();
    void itemDoubleClicked(QTableWidgetItem *item);
    void initFunSLot();
    void timeoutDone();

private:
    Ui::SetNamesWid *ui;
    SetNameDlg *mSetNameDlg;

    int mIndex;
    SetShm *mSetShm;
    sBusData *mPacket ;
    QTimer* mTimer;
};

#endif // SETNAMESWID_H
