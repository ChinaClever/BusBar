#include "emailwid.h"
#include "ui_emailwid.h"
#include "emailsetdlg.h"

static email_setInfo *gSet = NULL;
email_setInfo *email_get_setInfo(void) { return gSet; }


/**
 * @brief 列出所有用户邮件地址   用户邮件地址，还没有保存
 */
int usr_email_list(QStringList &email)
{
    email << "luozhiyong131@qq.com";

    return email.size();
}


EmailWid::EmailWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmailWid)
{
    ui->setupUi(this);

    gSet = new email_setInfo;
    initData();

    mEmailThread = new EmailThread(this);
}

EmailWid::~EmailWid()
{
    delete ui;
}


/**
 * @brief 初始化数据
 */
void EmailWid::initData(void)
{
    mSet = gSet;

    bool ret = sys_configFile_open();
    if(ret)
    {
        QString str = sys_configFile_readStr("Email_Server");
        if(str.isEmpty())
            str = Email_Server;
        mSet->server = str;

        str = sys_configFile_readStr("Email_UsrName");
        if(str.isEmpty())
            str = Email_UsrName;
        mSet->usr = str;

        str = sys_configFile_readStr("Email_Password");
        if(str.isEmpty())
            str = Email_Password;
        mSet->pwd = str;

        int port = sys_configFile_readInt("Email_Port");
        if(port <= 0)
            port = Email_Port;
        mSet->port = port;

        int ssl = sys_configFile_readInt("Email_ssl");
        if(ssl <= 0)
            mSet->ssl = false;
        else
            mSet->ssl = true;

        sys_configFile_close();
    }
    else
    {
        mSet->server = Email_Server;
        mSet->usr = Email_UsrName;
        mSet->pwd = Email_Password;
        mSet->port = Email_Port;
        mSet->ssl = false;
    }
}

void EmailWid::on_setBtn_clicked()
{
    EmailSetDlg dlg(this);
    dlg.setData(mSet);
    dlg.exec();
}

void EmailWid::on_sentBtn_clicked()
{
    QStringList recipient;
    QString emilAddr = ui->lineEdit->text(); //获取当前登录人的邮件
    if(!emilAddr.isEmpty())
        recipient << emilAddr;

    if(recipient.size() == 0)
    {
        bool ok;
        QString text = QInputDialog::getText(this,tr("邮件测试"),tr("请输入接收地址"),QLineEdit::Normal,NULL,&ok);
        if(ok && !text.isEmpty())
            recipient << text;
        else
            return ;
    }

    QString subject = tr("PDU测试邮件");
    QString body = tr("这是一份测试邮件!");

    Email email;
    email.sentEmail(recipient,subject,body);

    InfoMsgBox box(this,tr("邮件已发送至：%1，请等待!").arg(recipient.at(0)));
}
