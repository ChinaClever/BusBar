#ifndef SETTEMPWID_H
#define SETTEMPWID_H

#include "setloopwid.h"

class SetTempWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit SetTempWid(QWidget *parent = nullptr);

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
    int mBus;
    sBusData *mPacket ;
    QTimer* timer;
};

#endif // SETTEMPWID_H
