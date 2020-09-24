#ifndef BARCHARTS_H
#define BARCHARTS_H

#include "bardraw.h"

class BarCharts : public QWidget
{
    Q_OBJECT
public:
    explicit BarCharts(QWidget *parent = nullptr);
    ~BarCharts();

    void createBars(int num);
    void initXAxis(QVector<QString> &labels);
    void setPen(int id, QPen pen);
    void setBrush(int id, QColor c);

    void setXRange(int data);
    void setYRange(int data);
    void setData(int id, double data);

    void clearData();
    void replot() {mDraw->replot();}

signals:

public slots:


private:
    int mXrange, mYrange;
    BarDraw *mDraw;
    QList<QCPBars *> mBars;
};

#endif // BARCHARTS_H
