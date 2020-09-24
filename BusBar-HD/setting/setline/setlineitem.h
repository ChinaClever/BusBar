#ifndef SETLINEITEM_H
#define SETLINEITEM_H

#include <QWidget>
#include "QProgressBar"
#include "setthreshold/setthresholddlg.h"

namespace Ui {
class SetLineItem;
}

class SetLineItem : public QWidget
{
    Q_OBJECT

public:
    explicit SetLineItem(QWidget *parent = 0);
    ~SetLineItem();

    void initLine(int line) {mLine = line;}
    void updateBus(int bus) {mBus=bus;}

protected:
    void updateWidget(int bus, int line);
    void setProgressbarValue(QProgressBar *bar, sDataUnit *data, int index);
    void setProcessBarColor(QProgressBar *bar, QString color);

protected slots:
    void timeoutDone();
    void curBarClicked();
    void volBarClicked();

private:
    Ui::SetLineItem *ui;

    QTimer *timer;
    int mLine, mBus;
    sDataPacket *mPacket;
};

#endif // SETLINEITEM_H
