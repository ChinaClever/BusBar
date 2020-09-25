#include "myprogressbar.h"

MyProgressBar::MyProgressBar(QWidget* parent):QProgressBar(parent)
{

}

void MyProgressBar::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
}

/**
 * @brief 将进度条与数据包关联起来，并通过信号发送给阈值设置界面
 * bus总线编号  isCur不是电流，即电压   index当前电流或电压相数
 * @param data
 */
void MyProgressBar::setPacket(bool isCur,int index)
{
//    mBus = bus;
    mIsCur = isCur;
    mIndex = index;
}
