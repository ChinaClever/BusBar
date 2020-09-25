#ifndef APPTCPSERVER_H
#define APPTCPSERVER_H

#include <QThread>

class AppTcpServer : public QThread
{
    Q_OBJECT
    explicit AppTcpServer(QObject *parent = nullptr);
public:
    ~AppTcpServer();
    static AppTcpServer *bulid();

    void sent(uchar *data, int len);

signals:

protected:
    void run();

public slots:

private:
    bool isRun;
};

void android_sent(uchar *buf, int len);

#endif // APPTCPSERVER_H
