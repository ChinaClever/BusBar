#ifndef SETTEMMAIN_H
#define SETTEMMAIN_H

#include "settempwid.h"

namespace Ui {
class SetTemMain;
}

class SetTemMain : public QWidget
{
    Q_OBJECT

public:
    explicit SetTemMain(QWidget *parent = 0);
    ~SetTemMain();
    void updateWid(int index);
private:
    Ui::SetTemMain *ui;

    SetLineTem *mLineTem;
    SetTempWid *mLoopTem;
};

#endif // SETTEMMAIN_H
