#include "thdtablewid.h"
#include <QGridLayout>

ThdTableWid::ThdTableWid(QWidget *parent) : ComTableWid(parent)
{
    QGridLayout *layout = new QGridLayout(parent);
    layout->addWidget(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    initWid();
}

void ThdTableWid::initWid()
{
    QString title = tr("各次谐波含量THD(%)");
    QStringList header;

    for(int i = 0; i < 10; i++)
        header << QString("%1").arg(i + 1);

    initTableWid(header, 3, title);
}

int ThdTableWid::updateLine(QList<ushort> &datas, int row)
{
    QStringList list;

    for(int i=0; i<datas.size(); ++i)
    {
        double value = datas.at(i) / 100.0;
        list << QString::number(value,'f', 2);
        // setItemColor(row, i+1, unit->alarm[i]);
    }
    setTableRow(row, list);

    return ++row;
}

void ThdTableWid::updateData(QList<ushort> &datas)
{
    int row = 0, i=0;

    do{
        QList<ushort> list = datas.mid(i, 10);
        if(list.size()) {
            row = updateLine(list, row);
            i += 10;
        } else {
            break;
        }

    } while(i < datas.size());

    checkTableRow(row);
}

void ThdTableWid::updateData(ushort *buf, int len)
{
    QList<ushort> datas;

    for(int i=0; i<len; ++i)
        datas << buf[i];

    updateData(datas);
}
