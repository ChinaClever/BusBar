#include "logsignal.h"

LogSignal::LogSignal(QObject *parent) : QObject(parent)
{

}

LogSignal *LogSignal::get()
{
    static LogSignal *sington = new LogSignal();
    return sington;
}

void LogSignal::changeType(int type)
{
    emit logTypeSig(type);
}
