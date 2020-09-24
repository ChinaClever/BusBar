#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);

    void init(int port, bool isVerify=true);
    bool isConnect();

    void sendData(char *data);
    void sendData(uchar *data, int len);
    void sendData(const QByteArray &data);

    int readData(QString &ip, char *data);

signals:

protected:
    void landVerify(QTcpSocket *socket);

private slots:
    void newConnectSlot();
    void readMessage();
    void removeUserFormList();

private:
    QTcpServer *m_tcpServer;
    QMap<QString, QTcpSocket *> m_mapClient;
    QString mIP;
    bool mIsConnect, mIsVerify;
};

#endif // TCPSERVER_H
