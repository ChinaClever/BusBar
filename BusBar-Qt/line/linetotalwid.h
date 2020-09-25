#ifndef LINETOTALWID_H
#define LINETOTALWID_H

#include <QWidget>
#include "common.h"

namespace Ui {
class LineTotalWid;
}

class LineTotalWid : public QWidget
{
    Q_OBJECT

public:
    explicit LineTotalWid(QWidget *parent = 0);
    ~LineTotalWid();

protected:
    void updateData();

public slots:
    void busChangeSlot(int id);

protected slots:
    void initFunSLot();
    void timeoutDone();

private:
    Ui::LineTotalWid *ui;
    sTgObjData *mTgBus;
    QTimer *timer;
};

#endif // LINETOTALWID_H
