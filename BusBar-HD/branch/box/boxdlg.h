#ifndef BOXDLG_H
#define BOXDLG_H

#include <QDialog>
#include "boxtotalwid.h"
#include "boxlooptablewid.h"

namespace Ui {
class BoxDlg;
}

class BoxDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BoxDlg(QWidget *parent = 0);
    ~BoxDlg();

    void initBox(int bus, int box);
protected:
    void initWid(int bus, int box);

private slots:
    void on_pushButton_clicked();

private:
    Ui::BoxDlg *ui;
    BoxTotalWid *mTotalWid;
    BoxLoopTableWid *mLineWid;
    sBoxData *mData;
};

#endif // BOXDLG_H
