#ifndef LINETABLE_H
#define LINETABLE_H

#include "lineroadwid.h"
#include "comtablewid.h"

class LineTable : public ComTableWid
{
    Q_OBJECT
public:
    explicit LineTable(QWidget *parent = nullptr);

    void updateData(sBoxData &box);
protected:
    void initWid();
    int updateObj(sObjData &obj, int row);

signals:

public slots:
};

#endif // LINETABLE_H
