#ifndef EMAIL_H
#define EMAIL_H

#include "smtp.h"

#define  Email_Server 	"mail.china-clever.com"     //"smtp.qq.com" /*发件邮箱服务器：*/
#define  Email_UsrName   "test@china-clever.com"       //"pdu_email@foxmail.com"	/* 发送邮箱账号 */
#define  Email_Password   "clever123"	//"luozhiyong131" /*发送邮箱密码：*/
#define  Email_Port 	25 	/*发件邮箱端口号：*/
#define  Email_ToUsrName 	"pdu_email@qq.com" /*接收邮箱：*/

typedef struct
{
    QString usr; //用户名
    QString pwd;  //密码
    QString server; //发件邮箱服务器
    int  port; // 发件邮箱端口号
    bool ssl; // 是否启用ssl
}email_setInfo;


class Email : public QObject
{
    Q_OBJECT
public:
    explicit Email(QObject *parent = 0);
    ~Email();

    void sentEmail(QStringList &, QString &, QString &);

protected:
    void sentMessage(QStringList &, QString &, QString &);
    void sentMessageSsl(QStringList &, QString &, QString &);

signals:
    void finishedSig();

public slots:

private:
    email_setInfo *mSet;    
};

extern int usr_email_list(QStringList &email);
extern email_setInfo *email_get_setInfo(void);
#endif // EMAIL_H
