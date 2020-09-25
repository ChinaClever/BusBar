#ifndef SETRTUCMD_H
#define SETRTUCMD_H

#include "setshm.h"

class SetRtuCmd : public QThread
{
    Q_OBJECT
public:
    explicit SetRtuCmd(QObject *parent = nullptr);

     void send(sThresholdItem &item);

protected:
     void sendData(int busID, int addr, ushort reg, ushort len);
     void sendReg(int reg, sThresholdItem &item);
     int getReg(sThresholdItem &item);

};

#endif // SETRTUCMD_H
