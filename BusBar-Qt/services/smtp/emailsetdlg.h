#ifndef EMAILSETDLG_H
#define EMAILSETDLG_H

#include <QDialog>
#include "email.h"
#include "common.h"

namespace Ui {
class EmailSetDlg;
}

class EmailSetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit EmailSetDlg(QWidget *parent = 0);
    ~EmailSetDlg();
    void setData(email_setInfo *);

protected:
    void initWidget(void);
    bool dataCheck(void);
    void saveData(void);

private slots:
    void on_saveBtn_clicked();

    void on_checkBox_clicked(bool checked);

private:
    Ui::EmailSetDlg *ui;
    email_setInfo *mSet;
};

#endif // EMAILSETDLG_H
