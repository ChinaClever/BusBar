#include "linetable.h"
#include <QGridLayout>

LineTable::LineTable(QWidget *parent) : ComTableWid(parent)
{
    QGridLayout *layout = new QGridLayout(parent);
    layout->addWidget(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    initWid();
}

void LineTable::initWid()
{
    QString title = tr("");
    QStringList header;
    header << "L" << tr("负载率(%)") << tr("电压谐波含量(%)") << tr("电流谐波含量(%)");
    initTableWid(header, 3, title);
}


int LineTable::updateObj(sObjData &obj, int row)
{
    QStringList list;

    list << QString((char)('A' + row));

    double value = obj.pl[row];
    list << QString::number(value);

    value = obj.volThd[row] / 100.0;
    list << QString::number(value,'f', 2);

    value = obj.curThd[row] / 100.0;
    list << QString::number(value,'f', 2);

    setTableRow(row, list);

    return ++row;
}

void LineTable::updateData(sBoxData &box)
{
    for(int i=0; i<3; ++i) {
        updateObj(box.data, i);
    }
}
