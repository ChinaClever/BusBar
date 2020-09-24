#ifndef THDCHARTWID_H
#define THDCHARTWID_H

#include "barcharts.h"

class ThdChartWid : public QWidget
{
    Q_OBJECT
public:
    explicit ThdChartWid(QWidget *parent = nullptr);
    ~ThdChartWid();

    void updateData(ushort *buf, int len);
signals:

public slots:

private:
     BarCharts *mBarCharts;
};

#endif // THDCHARTWID_H
