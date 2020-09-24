#ifndef SETLINETEM_H
#define SETLINETEM_H

#include "setlinewid.h"

namespace Ui {
class SetLineTem;
}

class SetLineTem : public QWidget
{
    Q_OBJECT

public:
    explicit SetLineTem(QWidget *parent = 0);
    ~SetLineTem();

    void indexChanged(int index);

protected:
    void updateWid();
    void setTem(int id);

protected slots:
    void timeoutDone();

private slots:
    void on_temBtn_1_clicked();

    void on_temBtn_2_clicked();

    void on_temBtn_3_clicked();

private:
    Ui::SetLineTem *ui;
    QTimer *timer;
    int mBus;
    sDataUnit *startBoxTem;
};

#endif // SETLINETEM_H
