#ifndef BEEPTHREAD_H
#define BEEPTHREAD_H

#include <QtCore>
#include <QThread>


class BeepThread : public QThread
{
    Q_OBJECT
    explicit BeepThread(QObject *parent = nullptr);
public:
    static BeepThread *bulid();

    void beep();
    void longBeep();

protected:
    void run();

signals:

public slots:

private:
   bool isRun;
   int mSec;
};

#endif // BEEPTHREAD_H
