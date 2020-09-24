#include "emailsetdlg.h"
#include "ui_emailsetdlg.h"

EmailSetDlg::EmailSetDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailSetDlg)
{
    ui->setupUi(this);
    setWindowTitle(tr("SMTP设置"));
    mSet = NULL;

    ui->checkBox->setHidden(true); // 隐藏加密邮件功能
}

EmailSetDlg::~EmailSetDlg()
{
    delete ui;
}


/**
 * @brief 设置初值
 * @param email
 */
void EmailSetDlg::setData(email_setInfo *email)
{
    mSet = email;
    initWidget();
}

/**
 * @brief 窗口初始
 */
void EmailSetDlg::initWidget(void)
{
    ui->usrEdit->setText(mSet->usr);
    ui->pwdEdit->setText(mSet->pwd);
    ui->serEdit->setText(mSet->server);
    ui->portEdit->setText(QString::number(mSet->port));

    if(mSet->ssl)
        ui->checkBox->setChecked(true);
}

/**
 * @brief 数据验证
 * @return
 */
bool EmailSetDlg::dataCheck(void)
{
    QString str = ui->portEdit->text();

    bool ret = cm_isDigitStr(str);
    if(ret == false)
        CriticalMsgBox box(this, "发件箱端口号错误!");

    return ret;
}

/**
 * @brief 保存数据
 */
void EmailSetDlg::saveData(void)
{
    mSet->usr =  ui->usrEdit->text();
    sys_configFile_writeParam("Email_UsrName", mSet->usr);

    mSet->pwd = ui->pwdEdit->text();
    sys_configFile_writeParam("Email_Password", mSet->pwd);

    mSet->server = ui->serEdit->text();
    sys_configFile_writeParam("Email_Server", mSet->server);

    mSet->port = ui->portEdit->text().toInt();
    sys_configFile_writeParam("Email_Port", ui->portEdit->text());

    if(ui->checkBox->isChecked()) {
        mSet->ssl = true;
        sys_configFile_writeParam("Email_ssl", QString::number(1));
    } else {
        mSet->ssl = false;
        sys_configFile_writeParam("Email_ssl", QString::number(0));
    }
}

void EmailSetDlg::on_saveBtn_clicked()
{
    bool ret = dataCheck();
    if(ret) {
        this->close();
        saveData();
    }
}

void EmailSetDlg::on_checkBox_clicked(bool checked)
{
    int port = 25;
    if(checked)
        port = 465;
    ui->portEdit->setText(QString::number(port));
}
