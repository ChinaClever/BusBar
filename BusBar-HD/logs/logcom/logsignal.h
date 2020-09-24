#ifndef LOGSIGNAL_H
#define LOGSIGNAL_H

#include <QObject>

class LogSignal : public QObject
{
    Q_OBJECT
    explicit LogSignal(QObject *parent = nullptr);
public:
    static LogSignal* get();

    void changeType(int);

signals:
     void logTypeSig(int);

public slots:
};

#endif // LOGSIGNAL_H
