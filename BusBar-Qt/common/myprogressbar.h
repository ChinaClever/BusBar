#ifndef MYPROGRESSBAR_H
#define MYPROGRESSBAR_H

#include <QWidget>
#include <QMouseEvent>
#include <QProgressBar>
#include "datapacket.h"

class MyProgressBar : public QProgressBar
{
    Q_OBJECT

public:
    explicit MyProgressBar(QWidget* parent=0);
    void setPacket(bool isCur, int index);

signals:
    void clicked(bool isCur,int index);
    void clicked();

private :
//    int mBus;
    bool mIsCur;
    int mIndex;

protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // MYPROGRESSBAR_H
