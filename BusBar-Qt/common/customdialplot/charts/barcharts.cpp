#include "barcharts.h"

BarCharts::BarCharts(QWidget *parent) : QWidget(parent)
{
    mDraw = new BarDraw(this);
    mDraw->initXAxis();
    mDraw->initYAxis();

    QGridLayout *layout = new QGridLayout(parent);
    layout->addWidget(mDraw);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    setYRange(15);
}

/**
 * @brief 初始化
 */
void BarCharts::createBars(int num)
{
    for(int i=0; i<num; ++i) {
        QCPBars *bar = mDraw->createBar(QString::number(i+1));
        mBars.append(bar);
    }

    setXRange(num); // 调整X轴范围
}

void BarCharts::setPen(int id, QPen pen)
{
    // QPen pen;
    // pen.setWidthF(1.2);
    // pen.setColor(QColor(255,0,0));

    mBars.at(id)->setPen(pen);
}

void BarCharts::setBrush(int id, QColor c)
{
    // QColor c(255,0,0);
    mBars.at(id)->setBrush(c);
}

/**
 * @brief 设置X轴
 */
void BarCharts::initXAxis(QVector<QString> &labels)
{
    mDraw->initXAxis(labels);
}

/**
 * @brief 设置Y轴范围
 */
void BarCharts::setYRange(int data)
{
    static int range=0;
    if(range < data)
    {
        range = data ; // + data/5;
        mDraw->setYRange(range);
    }
}

void BarCharts::setXRange(int data)
{
    static int range=0;
    if(range < data)
    {
        range = data ; //+ data/5;
        mDraw->setXRange(range);
    }
}

void BarCharts::setData(int id, double data)
{
    QVector<double> ticks;
    ticks << id;

    QVector<double> vecData;
    vecData << data;

    setXRange(id);
    setYRange(data);
    mBars.at(id)->setData(ticks, vecData);
}

void BarCharts::clearData()
{
    for(int i=0; i<mBars.size(); ++i) {
        mBars.at(i)->clearData();
    }
}
