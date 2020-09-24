#include "interfacechangesig.h"
#include "beepthread.h"

InterfaceChangeSig::InterfaceChangeSig(QObject *parent) : QObject(parent)
{

}

InterfaceChangeSig *InterfaceChangeSig::get()
{
    static InterfaceChangeSig *sington = new InterfaceChangeSig();
    return sington;
}

void InterfaceChangeSig::changeType(int type)
{
    emit typeSig(type);
    BeepThread::bulid()->beep();
}
