#include "smtpssl.h"

//#define SMTP_SSL_DEBUG

#if  SSL_EN

SmtpSsl::SmtpSsl( const QString &user, const QString &pass, const QString &host, int port, int timeout )
{
    socket = new QSslSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected() ) );
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(errorReceived(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(disconnected()), this,SLOT(disconnected()));

    this->user = user;
    this->pass = pass;

    this->host = host;
    this->port = port;
    this->timeout = timeout;
}

void SmtpSsl::sendMail(const QString &from, const QStringList &to, const QString &subject, const QString &body)
{
    for(int recipCount = 0; recipCount < to.count(); recipCount++)
        message.append("To: " + to.at(recipCount) + "\n");

    message.append("From: " + from + "\n");
    message.append("Subject: " + subject + "\n");
    message.append("Mime-Version: 1.0;\n");
    //    message.append("Content-Type: text/html; charset=\"utf-8\";\n");
    //    message.append("Content-Transfer-Encoding: base64;\n");
    message.append("\n");

    message.append(body);
    //    message.append(body.toUtf8().toBase64());
    message.replace( QString::fromLatin1( "\n" ), QString::fromLatin1( "\r\n" ) );
    message.replace( QString::fromLatin1( "\r\n.\r\n" ),
                     QString::fromLatin1( "\r\n..\r\n" ) );
    this->from = from;
    rcpt = to.at(0);
    recips = to;
    x = 1;
    state = Init;
    socket->connectToHostEncrypted(host, port); //"smtp.gmail.com" and 465 for gmail TLS
    if (!socket->waitForConnected(timeout)) {
        qDebug() << socket->errorString();
    }

    t = new QTextStream( socket );
}

SmtpSsl::~SmtpSsl()
{
    delete t;
    delete socket;
}
void SmtpSsl::stateChanged(QAbstractSocket::SocketState socketState)
{
#ifdef SMTP_SSL_DEBUG
    qDebug() <<"stateChanged " << socketState;
#endif
}

void SmtpSsl::errorReceived(QAbstractSocket::SocketError socketError)
{
#ifdef SMTP_SSL_DEBUG
    qDebug() << "error " <<socketError;
#endif
}

void SmtpSsl::disconnected()
{
#ifdef SMTP_SSL_DEBUG
    qDebug() <<"disconneted";
    qDebug() << "error "  << socket->errorString();
#endif
}

void SmtpSsl::connected()
{
#ifdef SMTP_SSL_DEBUG
    qDebug() << "Connected ";
#endif
}

void SmtpSsl::readyRead()
{
    // SMTP is line-oriented
    QString responseLine;
    do
    {
        responseLine = socket->readLine();
        response += responseLine;
    }
    while ( socket->canReadLine() && responseLine[3] != ' ' );
    responseLine.truncate( 3 );
#ifdef SMTP_SSL_DEBUG
    qDebug() << "Server response code:" <<  responseLine;
    qDebug() << "Server response: " << response;
#endif
    if ( state == Init && responseLine == "220" )
    {
        // banner was okay, let's go on
        *t << "EHLO localhost" <<"\r\n";
        t->flush();

        state = HandShake;
    }
    //No need, because I'm using socket->startClienEncryption() which makes the SSL handshake for you
    /*else if (state == Tls && responseLine == "250")
    {
        // Trying AUTH
        qDebug() << "STarting Tls";
        *t << "STARTTLS" << "\r\n";
        t->flush();
        state = HandShake;
    }*/
    else if (state == HandShake && responseLine == "250")
    {
        socket->startClientEncryption();
        if(!socket->waitForEncrypted(timeout))
        {
#ifdef SMTP_SSL_DEBUG
            qDebug() << socket->errorString();
#endif
            state = Close;
        }
        //Send EHLO once again but now encrypted

        *t << "EHLO localhost" << "\r\n";
        t->flush();
        state = Auth;
    }
    else if (state == Auth && responseLine == "250")
    {
        // Trying AUTH
#ifdef SMTP_SSL_DEBUG
        qDebug() << "Auth";
#endif
        *t << "AUTH LOGIN" << "\r\n";
        t->flush();
        state = User;
    }
    else if (state == User && responseLine == "334")
    {
        //Trying User
#ifdef SMTP_SSL_DEBUG
        qDebug() << "Username";
#endif
        //GMAIL is using XOAUTH2 protocol, which basically means that password and username has to be sent in base64 coding
        //https://developers.google.com/gmail/xoauth2_protocol
        *t << QByteArray().append(user).toBase64()  << "\r\n";
        t->flush();

        state = Pass;
    }
    else if (state == Pass && responseLine == "334")
    {
        //Trying pass
#ifdef SMTP_SSL_DEBUG
        qDebug() << "Pass";
#endif
        *t << QByteArray().append(pass).toBase64() << "\r\n";
        t->flush();

        state = Mail;
    }
    else if ( state == Mail && responseLine == "235" )
    {
        // HELO response was okay (well, it has to be)

        //Apperantly for Google it is mandatory to have MAIL FROM and RCPT email formated the following way -> <email@gmail.com>
#ifdef SMTP_SSL_DEBUG
        qDebug() << "MAIL FROM:<" << from << ">";
#endif
        *t << "MAIL FROM:<" << from << ">\r\n";
        t->flush();
        state = Rcpt;
    }
    else if ( state == Rcpt && responseLine == "250" )
    {
        //Apperantly for Google it is mandatory to have MAIL FROM and RCPT email formated the following way -> <email@gmail.com>
        *t << "RCPT TO:<" << rcpt << ">\r\n"; //r
        t->flush();
        // state = Data;
        if(recips.isEmpty() || x == recips.count() )
        {
            state = Data;
        }
        else
        {
            if(x < recips.count())
            {
                rcpt = recips.at(x);
                x++;
                state = Rcpt;
            }
        }
    }
    else if ( state == Data && responseLine == "250" )
    {
        *t << "DATA\r\n";
        t->flush();
        state = Body;
    }
    else if ( state == Body && responseLine == "354" )
    {

        *t << message << "\r\n.\r\n";
        t->flush();
        state = Quit;
    }
    else if ( state == Quit && responseLine == "250" )
    {

        *t << "QUIT\r\n";
        t->flush();
        // here, we just close.
        state = Close;
        emit status( tr( "Message sent" ) );
    }
    else if ( state == Close )
    {
        deleteLater();
        return;
    }
    else
    {
#ifdef SMTP_SSL_DEBUG
        // something broke.
        QMessageBox::warning( 0, tr( "Qt Simple SMTP client" ), tr( "Unexpected reply from SMTP server:\n\n" ) + response );
        state = Close;
        emit status( tr( "Failed to send message" ) );
#endif
    }
    response = "";
}

#endif
