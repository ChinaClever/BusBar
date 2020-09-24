#include "setrtucmd.h"
#include "rtuthread.h"
extern RtuThread *rtu[4];

SetRtuCmd::SetRtuCmd(QObject *parent) : QThread(parent)
{

}

void SetRtuCmd::sendData(int busID, int addr, ushort reg, ushort len)
{
#if (SI_RTUWIFI == 0)
    if((busID == 0xff) || (addr == 0xff)){
        for(int i=0; i<4; ++i) {
            if(rtu[i]) rtu[i]->sendData(addr, reg, len);
        }
    } else {
        if(rtu[busID]) rtu[busID]->sendData(addr, reg, len);
    }
#elif (SI_RTUWIFI == 1)
      insertBusCmd(busID, addr, reg, len);
#endif
}

void SetRtuCmd::sendReg(int reg, sThresholdItem &item)
{
    sendData(item.bus, item.box, reg, item.max);
    sendData(item.bus, item.box, reg+1, item.min);
}

void SetRtuCmd::send(sThresholdItem &item)
{
    int reg=0;
    switch (item.type) {
    case 1: reg = VoltageMAX_L1 + item.num*2; break;
    case 2: reg = CurrentMAX_L1 + item.num*2; item.max*=10; item.min*=10; break;
    case 3: reg = temperatureMAX_1 + item.num*2; break;
    }
    sendReg(reg, item);
}
