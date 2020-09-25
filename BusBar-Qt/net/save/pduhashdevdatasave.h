#ifndef PDUHASHDEVDATASAVE_H
#define PDUHASHDEVDATASAVE_H

#include "pduhashcom.h"

typedef enum {
    PDU_CMD_VALUE =1,

    PDU_CMD_MIN, //最小值
    PDU_CMD_MAX, //最大值
    PDU_CMD_ALARM, //告警

    PDU_CDM_CRMIN,//临界最小值
    PDU_CMD_CRMAX, //临界最大值
    PDU_CMD_CRALARM, //临界告警
}PDU_CMD_DATA_TYPE;


typedef enum {
    PDU_CMD_NUM_SIZE, //数量
    PDU_CMD_CUR, //电流
    PDU_CMD_VOL, // 电压
    PDU_CMD_POW, // 功率
    PDU_CMD_ELE, // 电能
    PDU_CMD_PF, // 功率因素
    PDU_CMD_SW, // 开关状态 0 表示未启用    
    PDU_CMD_PL, // 负载率
    PDU_CMD_CTHD, // 电流谐波
    PDU_CMD_VTHD, // 电压谐波
}PDU_CMD_OBJ;


typedef enum {
    PDU_CMD_TEMP = 1, //温度
    PDU_CMD_HUM, // 湿度
    PDU_CMD_DOOR, //门禁
    PDU_CMD_WATER, //水浸
    PDU_CMD_SMOKE, //烟雾
}PDU_CMD_ENV_ENUM;

// 设备数据主功能码
typedef enum {
    PDU_CMD_STATUS=0,  //设备状态
    PDU_CMD_RATE, // 电压频率
    PDU_CMD_LINE, //相电参数
    PDU_CMD_OUTPUT,//输出位
    PDU_CMD_ENV, //环境数据
    PDU_CMD_DEVINFO,
    PDU_CMD_OUTPUTNAME,
    PDU_CMD_THD,
    PDU_CMD_VERSION,
}PDU_CMD_ID_DEV_DATA;


void pdu_hashDevData_save(sBoxData *dev, pdu_dev_data *data);



#endif // PDUHASHDEVDATASAVE_H
