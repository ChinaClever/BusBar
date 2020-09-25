#ifndef INTERFACECHANGESIG_H
#define INTERFACECHANGESIG_H

#include <QObject>

class InterfaceChangeSig : public QObject
{
    Q_OBJECT
    explicit InterfaceChangeSig(QObject *parent = nullptr);
public:
    static InterfaceChangeSig* get();

    void changeType(int);

signals:
     void typeSig(int);
};

#endif // INTERFACECHANGESIG_H
