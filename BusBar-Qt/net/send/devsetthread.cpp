#include "devsetthread.h"

DevSetThread::DevSetThread(QObject *parent) : QThread(parent)
{

}

DevSetThread *DevSetThread::bulid()
{
    static DevSetThread* sington = NULL;
    if(sington == NULL)
        sington = new DevSetThread();
    return sington;
}

void DevSetThread::insert(const dev_data_packet &pkt)
{
    sThresholdItem item;

    item.bus = pkt.num;
    item.box = pkt.addr;
    item.type = pkt.fn[0];
    item.num = pkt.fn[1];

    if(item.num) item.num--;
    else item.box = 0xff;

    int i=0, rate=1;
    if((item.type==1) || (item.type==3)) rate = 10;
    item.min = (pkt.data[i++]*256 + pkt.data[i++]) / rate;
    item.max = (pkt.data[i++]*256 + pkt.data[i++]) / rate;

    if(pkt.len > 4) {
        item.crmin = (pkt.data[i++]*256 + pkt.data[i++]) / rate;
        item.crmax = (pkt.data[i++]*256 + pkt.data[i++]) / rate;
    }

    SetThread::bulid()->append(item);
}

