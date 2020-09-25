#ifndef THDTABLEWID_H
#define THDTABLEWID_H

#include "comtablewid.h"

class ThdTableWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit ThdTableWid(QWidget *parent = nullptr);

    void updateData(ushort *buf, int len);
    void updateData(QList<ushort> &datas);

protected:
    void initWid();
    int updateLine(QList<ushort> &datas, int row);

signals:

public slots:
};

#endif // THDTABLEWID_H
