#ifndef SETKEY_H
#define SETKEY_H

#include <QDialog>
#include <QSignalMapper>

namespace Ui {
class SetKey;
}

class SetKey : public QDialog
{
    Q_OBJECT

public:
    explicit SetKey(QWidget *parent = 0, double value = 0, QString tit = "修改值:");
    ~SetKey();

    double getNuber();

public slots:
    void keyPress(int value);

protected:
    void initKey(void);

private slots:
    void on_timeSet_but_clicked();

    void on_quitBtn_clicked();

private:
    QSignalMapper *m;
    double mNuber;
private:
    Ui::SetKey *ui;
};

#endif // SETKEY_H
