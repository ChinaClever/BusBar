#ifndef BARDRAW_H
#define BARDRAW_H

#include "qcustomplot.h"

class BarDraw : public QWidget
{
    Q_OBJECT
public:
    explicit BarDraw(QWidget *parent = nullptr);
    ~BarDraw();

    QCPBars *createBar(const QString &);
    void initXAxis(QVector<QString> &);

    void initXAxis(void);
    void setXRange(int range);

    void initYAxis(void);
    void setYRange(int);
    void setYLabel(const QString &lab);

    void initLegend(void);
    void replot() {customPlot->replot();}

private:
    QCustomPlot *customPlot;
};

#endif // BARDRAW_H
