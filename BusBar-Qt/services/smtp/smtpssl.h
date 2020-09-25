#ifndef SMTPSSL_H
#define SMTPSSL_H

#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QSslSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QtWidgets/QMessageBox>
#include <QByteArray>
#include <QTcpSocket>

#define SSL_EN  0  // 开发板QT不支持SSL

#if  SSL_EN

class SmtpSsl : public QObject
{
    Q_OBJECT

public:
    SmtpSsl( const QString &user, const QString &pass,
          const QString &host, int port = 465, int timeout = 30000 );
    ~SmtpSsl();

    void sendMail( const QString &from, const QStringList &to,
                   const QString &subject, const QString &body );

signals:
    void status( const QString &);

private slots:
    void stateChanged(QAbstractSocket::SocketState socketState);
    void errorReceived(QAbstractSocket::SocketError socketError);
    void disconnected();
    void connected();
    void readyRead();

private:
    int timeout;
    QString message;
    QTextStream *t;

    QSslSocket *socket;

    QString from;
    QString rcpt;
    QStringList recips;
    QString response;
    QString user;
    QString pass;
    QString host;
    int x;
    int port;
    enum states{Tls, HandShake ,Auth,User,Pass,Rcpt,Mail,Data,Init,Body,Quit,Close};
    int state;

};

#endif

#endif // SMTPSSL_H
