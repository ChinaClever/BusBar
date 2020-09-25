#ifndef SETNAMEDLG_H
#define SETNAMEDLG_H

#include <QDialog>
namespace Ui {
class SetNameDlg;
}

class SetNameDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SetNameDlg(QWidget *parent = 0);
    ~SetNameDlg();

    void init(int bus, int box, int loop, const QString &name);
protected:
    bool save();
    bool saveToDev();

private slots:
    void on_saveBtn_clicked();
    void on_cancelBtn_clicked();

private:
    Ui::SetNameDlg *ui;
    int mBusId, mBox, mLoop;
};

#endif // SETNAMEDLG_H
