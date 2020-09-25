/*
 * email.cpp
 *  发送邮件
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "email.h"
#include "smtpssl.h"

Email::Email(QObject *parent) : QObject(parent)
{
    mSet = email_get_setInfo();
}

Email::~Email()
{

}


void Email::sentEmail(QStringList &recipient,QString &subject,QString &body)
{
    if(mSet)
    {
        if(mSet->ssl) {
            sentMessageSsl(recipient,subject,body);
        } else {
            sentMessage(recipient,subject,body);
        }
    }
    else
        qDebug() << " sentEmail err";
}


void Email::sentMessageSsl(QStringList &to, QString &subject, QString &body)
{
    QString from = mSet->usr;

#if SSL_EN
    /* 简单加密传输 */
    SmtpSsl* smtp = new SmtpSsl(mSet->usr, mSet->pwd, mSet->server, mSet->port);
    smtp->sendMail(from, to, subject,body);
#endif
}


/**
 * @brief 邮件发送
 * @param list
 * @param subject
 * @param body
 * @return
 */
void Email::sentMessage(QStringList &to, QString &subject, QString &body)
{
    QString from = mSet->usr;

    Smtp *smtp = new Smtp(mSet->server,mSet->usr,mSet->pwd,from,mSet->port,
                          to,subject,body);
    connect(smtp, SIGNAL(finishedSig()),this, SIGNAL(finishedSig()));
}


