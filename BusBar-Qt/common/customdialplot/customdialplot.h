#ifndef CUSTOMDIALPLOT_H
#define CUSTOMDIALPLOT_H

#include <QWidget>

class CustomDialPlot : public QWidget
{
    Q_OBJECT

public:
    explicit CustomDialPlot(QWidget *parent = 0);
    //设置表盘刻度范围
    void setRange(int nMinValue=0,int nMaxValue=100);
    //获取最小值
    int getMin()const;
    //获取最大值
    int getMax()const;

    //更新表盘显示值
    void setValue(double dValue );
    void setColor(QPainter &pPainter, double dValue);


    //设置表盘表示数据单位
    void setUnit(const QString& strUnit);


protected:
    virtual void paintEvent(QPaintEvent *);

private:
    //检测设置范围是否符合规则
    bool _isRangeCorrect(int nMinValue,int nMaxValue);

    //绘制表盘背景
    void _drawBackground(QPainter *pPainter);
    //绘制表盘
    void _drawDial(QPainter*pPainter);
    //绘制表盘刻度
    void _drawTicks(QPainter*pPainter);
    //绘制指针
    void _drawPointer(QPainter* painter);
    //绘制显示值和单位
    void _drawValue(QPainter* painter);

    bool isFirst;
private:
    //仪表盘最外圈半径
    int m_nRadius;
    QPointF m_centerPointF;
    //表盘刻度最小值
    int m_nMinValue;
    //表盘刻度最大值
    int m_nMaxValue;
    //当前值
    double m_dCurrentValue;
    //单位
    QString m_strUnit;

    int mType;//标志位，根据标志位决定颜色
};

#endif // CUSTOMDIALPLOT_H
