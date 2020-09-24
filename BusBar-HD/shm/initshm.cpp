#include "initshm.h"
#include "dbdevname.h"

InitShm::InitShm(QObject *parent) : QThread(parent)
{
    shm = get_share_mem(); // 获取共享内存
}

/**
 * @brief 初始化插接箱数量
 */
void InitShm::initBoxNum()
{
    for(int i=0; i<BUS_NUM; ++i) {
        int boxNum = getBoxNum(i);  //插接箱数
        if(boxNum < 0) boxNum = 18;
        shm->data[i].boxNum = boxNum; // 18个插接箱

        int rateCur = getRateCur(i); // rateCur
        if(rateCur < 0) rateCur = 2600;
        shm->data[i].box[0].ratedCur = rateCur; // 额定电流
    }
}

void InitShm::initThresholdUnit(int id, sDataUnit &unit, int max)
{
    unit.max[id] = max;
    unit.crMax[id] = max;
}


void InitShm::initBoxThreshold()
{    
    for(int i=0; i<BUS_NUM; ++i) //每个母线下都有
    {
        sBusData *busData = &(shm->data[i]); //母线
        for(int j=1; j<=busData->boxNum; ++j) //值init有效位
        {
            sBoxData *box = &(busData->box[j]); //插接葙
            for(int k=0; k<LINE_NUM_MAX; ++k) //三相
            {
                initThresholdUnit(k, box->data.cur, 3200);
                initThresholdUnit(k, box->data.vol, 480);
            }

            for(int k=0; k<SENSOR_NUM; ++k) //两个传感器
            {
                initThresholdUnit(k, box->env.tem, 99);
            }
        }
    }
}

void InitShm::initBusThreshold()
{
    for(int i=0; i<BUS_NUM; ++i)
    {
        sBoxData *bus = &(shm->data[i].box[0]);
        for(int k=0; k<3; ++k)
        {
            initThresholdUnit( k, bus->data.vol, 275);
            initThresholdUnit( k, bus->data.cur, 6000);
            initThresholdUnit( k, bus->env.tem, 99);
        }
    }
}

void InitShm::initThreshold()
{
    initBusThreshold();
    initBoxThreshold();
}

void InitShm::initNameUnit(int bus, int type, int num, char *buf, const QString &name)
{
    QString  str = DbDevName::bulid()->getName(bus, type, num); //数据库中取数据
    if(str.isEmpty()) //没有找到就创建并保存
    {
        DbNameItem item;
        item.bus = bus;
        item.type = type;
        item.num = num;
        item.name = name;
        DbDevName::bulid()->saveItem(item);
        str = name;
    }

    QByteArray ba = str.toLatin1();
    char *mm = ba.data();
    strcpy(buf,mm);
}


void InitShm::initBusName()
{
    for(int i=0; i<BUS_NUM; ++i)
    {
        sBusData *busData = &(shm->data[i]);
        initNameUnit(i, 1, 0, busData->busName, QString("BUS-%1").arg(i+1)); //母线名称各处统一
    }
}

void InitShm::initBoxName()
{
    for(int i=0; i<BUS_NUM; ++i)
    {
        sBusData *busData = &(shm->data[i]);
        for(int j=1; j<BOX_NUM; ++j)
        {
            sBoxData *box = &(busData->box[j]);
            initNameUnit(i, 2, j, box->boxName, QString("iBox-%1").arg(j));//插接箱名称各处统一
        }
    }
}

void InitShm::initLoopName()
{
    for(int i=0; i<BUS_NUM; ++i)
    {
        sBusData *busData = &(shm->data[i]);
        for(int j=1; j<=busData->boxNum; ++j)
        {
            sBoxData *box = &(busData->box[j]);
            for(int k=0; k<LINE_NUM; ++k) {
                char *loop = box->loopName[k];
                int num = j*LINE_NUM + k;
                initNameUnit(i, 3, num, loop, QString("loop-%1").arg(k+1)); //回路名称
            }
        }
    }
}

void InitShm::initName()
{
    initBusName();
    initBoxName();
    initLoopName();
}

void InitShm::run()
{
    initBoxNum();  //统一ini 与共享内存的数组有效长度
    initName(); //统一SQL 与功效内存内neme
    initThreshold(); //统一数据标准
}
