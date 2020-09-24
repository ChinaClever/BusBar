#include "emailthread.h"
extern QString get_email_str();

EmailThread::EmailThread(QObject *parent) : QThread(parent)
{
    isRun = isStart = false;
    mEmail = new Email(this);
    connect(mEmail, SIGNAL(finishedSig()),this, SLOT(finishedSlot()));

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    mTimer->start(2000);

//    initFunction();
}

EmailThread::~EmailThread()
{
    isRun = false;
    wait();
}

void EmailThread::initFunction()
{
    start();
}

/**
 * @brief 获取发送内容
 * @param subject 标题
 * @param body  内容
 * @return true 表示需要发送邮件
 */
bool EmailThread::getMessage(QString &subject, QString &body)
{
    bool ret = true;

    subject = tr("IDC-BUS告警邮件");
    body = get_email_str();
    if(body.isEmpty())
        ret = false;

    return ret;
}

/**
 * @brief 邮件发送完成响应函数
 */
void EmailThread::finishedSlot()
{
    isStart = false;
}

/**
 * @brief 发送邮件
 */
void EmailThread::setEmail(void)
{
    QString subject,body;
    bool ret = getMessage(subject,body);
    if(ret)
    {
        QStringList recipient;
        usr_email_list(recipient);

        if(recipient.size() > 0)
        {
            isStart = true;
            mEmail->sentEmail(recipient,subject,body);
        }
    }
}

/**
 * @brief 定时器发送邮件
 */
void EmailThread::timeoutDone()
{
    if(isStart == false)
        setEmail();
}

/**
 * @brief 线程发送邮件
 */
void EmailThread::run(void)
{
    isRun = true;
    while(isRun)
    {
        timeoutDone();
        msleep(100);
    }
}
