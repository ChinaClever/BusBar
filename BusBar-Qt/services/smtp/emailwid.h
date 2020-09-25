#ifndef EMAILWID_H
#define EMAILWID_H

#include <QWidget>
#include "emailthread.h"

namespace Ui {
class EmailWid;
}

class EmailWid : public QWidget
{
    Q_OBJECT

public:
    explicit EmailWid(QWidget *parent = 0);
    ~EmailWid();

protected:
    void initData(void);

private slots:
    void on_setBtn_clicked();
    void on_sentBtn_clicked();

private:
    Ui::EmailWid *ui;
    EmailThread *mEmailThread;
    email_setInfo *mSet;
};

#endif // EMAILWID_H
