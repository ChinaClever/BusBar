#ifndef SETPOWMAIN_H
#define SETPOWMAIN_H

#include <QWidget>
#include "setlooppowerwid.h"
#include "setline/setlinetem.h"

namespace Ui {
class SetPowMain;
}

class SetPowMain : public QWidget
{
    Q_OBJECT

public:
    explicit SetPowMain(QWidget *parent = nullptr);
    ~SetPowMain();

    void updateWid(int index);
private:
    Ui::SetPowMain *ui;

    SetLineTem *mLineTem;
    SetLoopPowerWid *mLoopPowerTem;
};

#endif // SETPOWMAIN_H
