/*
 * devDataType.h
 *
 *  Created on: 2016年8月1日
 *      Author: lzy
 */

#ifndef DEVDATA_DEVDATATYPE_H_
#define DEVDATA_DEVDATATYPE_H_
#include "datadone/netpackdata.h"

/*数据单元结构体*/
typedef struct
{
    ushort		*value; // 当前值

    ushort 		*min; //最小值
    ushort 		*max; //最大值
    uchar 		*alarm; // 告警状态

    ushort 		*crMin; //临界最小值
    ushort 		*crMax; //临界最大值
    uchar 		*crAlarm; //临界告警状态
}_devDataUnit;

typedef struct
{
    ushort 		len; // 长度
    _devDataUnit		vol; // 电流
    _devDataUnit 		cur; //电压

    uint 		*pow; // 功率
    uint 		*ele; // 电能

    ushort 		*pf; //功率因素
    uchar 		*sw; //开关状态

    ushort 		*rate; //电压频率
    ushort 		*apPow; // 视在功率

    ushort		*pl; //
    ushort		*curThd; //
    ushort		*volThd; //
}_devDataObj;

typedef struct
{
    uchar 		len; // 长度
    _devDataUnit		tem; // 温度
    _devDataUnit 		hum; //湿度
    uchar 		*door; // 门禁
    uchar 		*water; // 水浸
    uchar 		*smoke; //烟雾
}_envDataObjct;

typedef struct
{
    _devDataObj		    line; // 输入相
    _devDataObj		    loop; // 输入相
    _devDataObj 		output; //输出位
    _envDataObjct 		env; // 环境
}pduDevData;


#endif /* DEVDATA_DEVDATATYPE_H_ */
