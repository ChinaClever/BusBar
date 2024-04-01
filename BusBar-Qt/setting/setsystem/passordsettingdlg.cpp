#include "passordsettingdlg.h"
#include "ui_passordsettingdlg.h"

PassordSettingDlg::PassordSettingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PassordSettingDlg)
{
    ui->setupUi(this);
    com_setBackColour(tr("设置界面"), this);
    initLanguage();
}

PassordSettingDlg::~PassordSettingDlg()
{
    delete ui;
}
void PassordSettingDlg::initLanguage()
{
    if(gLanguage == 0){
        ui->groupBox->setTitle("密码设置");
        ui->label->setText("原密码：");
        ui->label_2->setText("新密码：");
        ui->lineEdit->setPlaceholderText("请输入原密码");
        ui->pushButton_2->setText("保存");
        ui->pushButton_3->setText("取消");
    }else{
        ui->groupBox->setTitle("Password setting");
        ui->label->setText("Old password:");
        ui->label_2->setText("New password:");
        ui->lineEdit->setPlaceholderText("Please enter the original password");
        ui->pushButton_2->setText("Save");
        ui->pushButton_3->setText("Cancel");
    }
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
        if(gLanguage == 0) QMessageBox::information(this,"information",tr("密码设置成功，点击确定退出！"),tr("确定"));
        else QMessageBox::information(this,"information",tr("The password is set successfully,click Confirm to exit!"),tr("Confirm"));
        this->close();
    }
    else{
        if(gLanguage == 0) QMessageBox::information(this,"information","原密码输入错误，请重新输入！","确定");
        else QMessageBox::information(this,"information","The original password was entered incorrectly,please re-enter it!","Confirm");
    }
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

