#include "setloopwid.h"

SetLoopWid::SetLoopWid(QWidget *parent) : ComTableWid(parent)
{
    mDc = 1;
    mBus = 0;
    mPacket =  &(get_share_mem()->data[mBus]);
    initWid();

    timer = new QTimer(this);
    timer->start(2000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}


void SetLoopWid::initWid()
{
    QString title = tr("回路电流");
    QStringList header;
    header<< tr("插接箱");

    if(mDc){ //交流9个
        for(int i = 0; i < LINE_NUM; ++i)
            header << QString((char)('A' + i%3))+ QString("%1").arg(i/3 + 1);
    }else{ //直流4个
        for(int i = 0; i < 4; i++)
            header << "D" + QString("%1").arg(i + 1);
    }

    initTableWid(header, 1, title);
}



void SetLoopWid::checkBus(int index)
{
//    if(mBus != index) {
//        mBus = index;
    mPacket = &(get_share_mem()->data[index]);
//    }

    int dc = mPacket ? mPacket->box[0].dc : 0;
    if(mDc != dc ) {
        mDc = dc;
        initWid();
    }
}


int SetLoopWid::updateDev(sBoxData *dev, int row)
{
    if(dev->offLine)
    {
        QStringList list;
        list << dev->boxName;

        sDataUnit *unit = &(dev->data.cur);
        int line = dev->data.lineNum;
        for(int i=0; i<line; ++i)
        {
            double value = unit->value[i] / COM_RATE_CUR;
            list << QString::number(value,'f', 1) + "A";
            setItemColor(row, i+1, unit->alarm[i]);
        }

        setTableRow(row, list);
    }

    return ++row;
}

/**
 * @brief 数据更新入口函数
 */
void SetLoopWid::updateData()
{
    int row = 0;

    for(int i=1; i<=mPacket->boxNum; ++i)
    {
        sBoxData *box = &(mPacket->box[i]);
        row = updateDev(box, row);
    }

    checkTableRow(row);
}

void SetLoopWid::timeoutDone()
{
    checkBus(mBus);
    updateData();
}


void SetLoopWid::itemClicked(QTableWidgetItem *it)
{
    if(it->text().compare("---") == 0) return;  //为空不设置
    static int i = 0;//防止弹出多次对话框
    i++;
    if(i % 3 == 1)
    {

        int column = it->column();
        if(column > 0)
        {
            BeepThread::bulid()->beep();
            sThresholdItem item;
            item.bus = mBus;
            item.box = it->row()+1;
            item.num = column-1;
            item.type = 2;

            SetThresholdDlg dlg(this);
            dlg.move(0,0);
            dlg.set(item);
            dlg.exec();
            i = 1;
        }
    }
}
