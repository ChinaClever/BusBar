#include "beepthread.h"
#include "gpio_dev.h"
#include "modbus/serialport/serial_portset.h"

BeepThread::BeepThread(QObject *parent) : QThread(parent)
{
    isRun = false ;
    gpio_init();

#if ARM_LINUX == 2 //第一次运行时要执行
    system("echo 129 > /sys/class/gpio/export");
    system("echo \"out\" > /sys/class/gpio/gpio129/direction");
#endif
}

BeepThread *BeepThread::bulid()
{
    static BeepThread* sington = NULL;
    if(sington == NULL)
        sington = new BeepThread();
    return sington;
}

void BeepThread::beep()
{
    mSec = 100;
    start();
}

void BeepThread::longBeep()
{
    mSec = 650;
    start();
}

void BeepThread::run()
{

    if(isRun == false)
    {
        isRun  = true;
#if ARM_LINUX == 1
        msleep(50);
        gpio_on(2);
        msleep(mSec);
        gpio_off(2);
        msleep(300);
#elif ARM_LINUX == 2
        system("echo \"1\" > /sys/class/gpio/gpio129/value");
        msleep(mSec);
        system("echo \"0\" > /sys/class/gpio/gpio129/value");
        msleep(400);
#endif
        isRun  = false;
    }
}
