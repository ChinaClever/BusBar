#ifndef TESTDLG_H
#define TESTDLG_H

#include <QDialog>

namespace Ui {
class TestDlg;
}

class TestDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit TestDlg(QWidget *parent = 0);
    ~TestDlg();
    
private:
    Ui::TestDlg *ui;
};

#endif // TESTDLG_H
