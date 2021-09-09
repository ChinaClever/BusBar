#ifndef SETLOOPPOWERWID_H
#define SETLOOPPOWERWID_H

#include "comtablewid.h"
#include "setthreshold/setthresholddlg.h"

class SetLoopPowerWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit SetLoopPowerWid(QWidget *parent = nullptr);

    void setBus(int bus) {mBus=bus;}

protected slots:
    void timeoutDone();
    void itemClicked(QTableWidgetItem* it);

protected:
    void initWid();
    void checkBus(int index);
    int updateDev(sBoxData *dev, int row);
    void updateData();

private:
    int mDc, mBus;
    sBusData *mPacket ;
    QTimer* timer;
};

#endif // SETLOOPPOWERWID_H
