#ifndef UDPHEARTBEAT_H
#define UDPHEARTBEAT_H

#include "udpsentsocket.h"

class UdpHeartBeat : public QThread
{
    Q_OBJECT
public:
    explicit UdpHeartBeat(QObject *parent = 0);
    ~UdpHeartBeat();

    void startSent(void);

protected:
    void run(void);
     void heartbeatPacket(void);

signals:

public slots:
    void timeoutDone(void);

private:
    bool isRun;
    QTimer *timer;

    UdpSentSocket *mSocket;
};
void set_hb_IP(const QStringList &ips);

#endif // UDPHEARTBEAT_H
