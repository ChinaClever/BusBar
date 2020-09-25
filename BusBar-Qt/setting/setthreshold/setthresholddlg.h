#ifndef SETTHRESHOLDDLG_H
#define SETTHRESHOLDDLG_H

#include <QDialog>
#include "setshm.h"

namespace Ui {
class SetThresholdDlg;
}

class SetThresholdDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SetThresholdDlg(QWidget *parent = 0);
    ~SetThresholdDlg();


    void set(sThresholdItem &item);

protected:
    bool checkData();
    void setTitle(sThresholdItem &item);
    void initSpinBox(sThresholdItem &item);

private slots:
    void on_saveBtn_clicked();
    void on_cancelBtn_clicked();

private:
    Ui::SetThresholdDlg *ui;
    sThresholdItem mItem;
};

#endif // SETTHRESHOLDDLG_H
