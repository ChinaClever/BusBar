#ifndef LINEWID_H
#define LINEWID_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTimer>
#include "linetotalwid.h"
#include "customdialplot.h"
#include "linetable.h"

namespace Ui {
class LineWid;
}

class LineWid : public QWidget
{
    Q_OBJECT

public:
    explicit LineWid(QWidget *parent = 0);
    ~LineWid();

signals:
    void busChangedSig(int);

protected:
    void initFun();
    void initWid();
    void initTotalWid();
    void updateTotalWid();
    void updatePlot();

protected slots:
    void timeoutDone();
    void indexChanged(int index);
    void interfaceChangedSlot(int id);

private slots:
    void on_thdBtn_clicked();

private:
    Ui::LineWid *ui;

    QTimer *timer;
    LineTable *mLineTable;
    LineTotalWid *mTotalWid;
    LineRoadWid *mLine[3];
    sBusData *mData;


    CustomDialPlot *mCurPlot;
    CustomDialPlot *mVolPlot;
    CustomDialPlot *mPwPlot;
    CustomDialPlot *mPfPlot;
    int mIndex;
    bool isRun;
};

#endif // LINEWID_H
