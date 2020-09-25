#include "passordsettingdlg.h"
#include "ui_passordsettingdlg.h"

PassordSettingDlg::PassordSettingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PassordSettingDlg)
{
    ui->setupUi(this);
    com_setBackColour(tr("设置界面"), this);
}

PassordSettingDlg::~PassordSettingDlg()
{
    delete ui;
}

/**
 * @brief  保存
 */
void PassordSettingDlg::on_pushButton_2_clicked()
{
    bool ret = checkJurisdiction();
    QString newPassword = ui->lineEdit_2->text();

    if(ret)
    {
        BeepThread::bulid()->beep();
        sys_configFile_writeParam("password",newPassword);
        QMessageBox::information(this,"information",tr("密码设置成功，点击确定退出！"),tr("确定"));
        this->close();
    }
    else
        QMessageBox::information(this,"information","原密码输入错误，请重新输入！","确定");
}

bool PassordSettingDlg::checkJurisdiction()
{
    QString old = ui->lineEdit->text();
    if(old == getPassword())
        return true;

    return false;
}

void PassordSettingDlg::on_pushButton_3_clicked()
{
    this->close();
}

