#include "testdlg.h"
#include "ui_testdlg.h"

TestDlg::TestDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestDlg)
{
    ui->setupUi(this);
}

TestDlg::~TestDlg()
{
    delete ui;
}
