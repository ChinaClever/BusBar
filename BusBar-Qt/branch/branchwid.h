#ifndef BRANCHWID_H
#define BRANCHWID_H

#include <QWidget>
#include "boxtablewidget.h"

namespace Ui {
class BranchWid;
}

class BranchWid : public QWidget
{
    Q_OBJECT

public:
    explicit BranchWid(QWidget *parent = 0);
    ~BranchWid();

signals:
    void busChangedSig(int);

private:
    Ui::BranchWid *ui;
    BoxTableWidget *mTableWid;
};

#endif // BRANCHWID_H
