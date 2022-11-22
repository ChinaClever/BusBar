#include "settempwid.h"
#include <QGridLayout>
SetTempWid::SetTempWid(QWidget *parent) : ComTableWid(parent)
{
    mBus = 0;
    mPacket =  &(get_share_mem()->data[mBus]);
    initWid();
    QGridLayout *gridLayout = new QGridLayout(parent);//控制ToolBox自适应
    gridLayout->addWidget(this);

    timer = new QTimer(this);
    timer->start(2000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}


void SetTempWid::initWid()
{
    QString title = tr("温度");
    QStringList header;
    header<< tr("插接箱");

    for(int i=0; i<SENSOR_NUM; ++i)
        header << tr("温度") + QString::number(i+1);

    initTableWid(header, 1, title);
}


void SetTempWid::checkBus(int index)
{
//    if(mBus != index) {
//        mBus = index;
    mPacket = &(get_share_mem()->data[index]);
//    }
    clearTable();
}


int SetTempWid::updateDev(sBoxData *dev, int row)
{
    if(dev->offLine)
    {
        QStringList list;
        list << dev->boxName;

        sDataUnit *unit = &(dev->env.tem);
        for(int i=0; i<SENSOR_NUM; ++i)
        {
            double value = unit->value[i] / COM_RATE_TEM;
            list <<  QString::number(value) + "℃";
            setItemColor(row, i+1, unit->alarm[i]);
        }
        setTableRow(row, list);
    }

    return ++row;
}

/**
 * @brief 数据更新入口函数
 */
void SetTempWid::updateData()
{
    int row = 0;

    for(int i=1; i<=mPacket->boxNum; ++i)
    {
        sBoxData *box = &(mPacket->box[i]);
        row = updateDev(box, row);
    }

    checkTableRow(row);
}

void SetTempWid::timeoutDone()
{
    checkBus(mBus);
    updateData();
}


void SetTempWid::itemClicked(QTableWidgetItem *it)
{
    if(it->text().compare("---") == 0) return;  //为空不设置
    int column = it->column();
    if(column > 0)
    {
        BeepThread::bulid()->beep();
        sThresholdItem item;
        item.bus = mBus;
        item.box = it->row()+1;
        item.num = column-1;
        item.type = 3;

        SetThresholdDlg dlg(this);
        dlg.move(0,0);
        dlg.set(item);
        dlg.exec();
    }

}
