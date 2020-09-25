#ifndef SETSHM_H
#define SETSHM_H

#include <QObject>
#include <QSettings>
#include "dbdevname.h"

struct sThresholdItem{
    int id;
    int type; // 阈值类型 1 电压阈值  2 电流阈值   3温度
    int bus; // 母线编号 0~3
    int box; // 插接箱
    int num; // 编号
    int min, crmin, crmax, max; // 最小值，临界下限  临界上限  最大值
};

class SetShm : public QObject
{
    Q_OBJECT
public:
    explicit SetShm(QObject *parent = 0);

    void setItem(sThresholdItem &item);
    void setName(DbNameItem &item); // 名称设置
    void setLineRatedCur(int index ,int data); //设置母线额定电流
    void setLineBoxNum(int index,int num); //设置母线插接箱数量

protected:
    void setThresholdUnit(int id, sThresholdItem &item, sDataUnit &unit);
    void saveItem(sThresholdItem &item); // 单一设置

    void setVolAll(sThresholdItem &item); // 设置成有主路电压
    void setLoopCurAll(sThresholdItem &item); // 设置所有回路电流
    void setLoopTempAll(sThresholdItem &item); // 设置所有插接箱温度

    void setLineCurAll(sThresholdItem &item); // 设置成有主路电流
    void setLineTempAll(sThresholdItem &item); // 设置始插箱温度
signals:

public slots:

private:
    sDataPacket *shm;
};

#endif // SETSHM_H
