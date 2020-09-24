#ifndef HOMEWID_H
#define HOMEWID_H

#include <QWidget>
#include "homeboxwid.h"

namespace Ui {
class HomeWid;
}

class HomeWid : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWid(QWidget *parent = 0);
    ~HomeWid();

protected:
    void initFun();
    void initWidget();
    bool checkBoxBaseNum();
    void setBoxBaseNum();

signals:
    void busChangedSig(int);

public slots:
    void timeoutDone();
    void interfaceChangedSlot(int);
     void busChangedSlot(int);
    void on_upBtn_clicked();
    void on_downBtn_clicked();

private:
    Ui::HomeWid *ui;
    QTimer *timer;

    int mBaseNum;
    sBusData *mBusData;
    HomeBoxWid *mBoxWid[BOX_NUM];
    bool isRun;
    int mMaxNum;
};

#endif // HOMEWID_H
