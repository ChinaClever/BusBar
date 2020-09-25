#ifndef TCPSENT_H
#define TCPSENT_H

#include "tcpclient.h"

class TcpSent : public QThread
{
    Q_OBJECT
public:
    explicit TcpSent(QObject *parent = 0);

    void newConnect(const QString &host); //连接服务器
protected:
    bool newConnect(void);
    bool sentCheck(void);
    void sentData(void);

signals:

protected slots:
    void timeoutDone();

private:
    TcpClient *mTcpClient;
    QTimer *timer;

    QString mHost;
    QReadWriteLock  *mLock;
    QQueue<QByteArray>  *mArrayQue;
};
void tcp_queue_append(uchar *buf, int len);
#endif // TCPSENT_H
