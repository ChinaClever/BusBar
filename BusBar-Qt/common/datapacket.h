#ifndef DATAPACKET_H
#define DATAPACKET_H
/*
 * datapacket.h
 * 数据包结构体定义思路
 * 	1、数据包中包含四条母线数据
 * 	2、一条母线包括最多20个插接箱数据
 * 	LINE_NUM、一个插接箱包括最多3个输出位
 * 	4、一个输出位包括基本的数据结体
 *
 *  Created on: 2017年5月24日
 *      Author: Lzy
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QtCore>

#define LINE_NUM 9 // 三相
#define LINE_NUM_MAX 9 // 三相
#define BOX_NUM 20 // 插接箱数量
#define BUS_NUM 4  // 四条母线
#define NAME_LEN	32 // 名称最大长度
#define SENSOR_NUM 4  // 二个传感器

#define SRC_DATA_LEN_MAX 1024 //一包数据最长 —— 用于对接动环

/**
 * 统计数据结构体
 */
typedef struct _sTgObjData {
    int vol; // 电压
    int cur;  // 电流
    int pow; // 功率

    int ele; // 电能
    int pf; // 功率因素
    int apPow; // 袖在功率
    int tem;
}sTgObjData;

typedef struct _sLineTgObjData {
    ushort vol[3]; // 电压
    ushort cur[3];  // 电流

    uint pow[3]; // 功率
    uint ele[3]; // 电能
    ushort pf[3]; // 功率因素
    uint apPow[3]; // 袖在功率
    uint reactivePower[3]; // 无功功率
}sLineTgObjData;

/**
 * 数据单元：包括当前值，阈值，临界值，告警状态等
 */
typedef struct _sDataUnit {
    ushort value[LINE_NUM_MAX]; // 值
    ushort min[LINE_NUM_MAX]; // 最小值
    ushort max[LINE_NUM_MAX]; // 最大值
    uchar alarm[LINE_NUM_MAX]; // 告警值 0表示未告警  1表示已告警 2表示已记录

    ushort crMin[LINE_NUM_MAX]; // 临界最小值
    ushort crMax[LINE_NUM_MAX]; // 临界最大值
    uchar crAlarm[LINE_NUM_MAX]; // 临界告警值
}sDataUnit;

/**
 * 数据单元：包括当前值，阈值，临界值，告警状态等
 */
typedef struct _sDataPowUnit {
    uint value[LINE_NUM_MAX]; // 值
    uint min[LINE_NUM_MAX]; // 最小值
    uint max[LINE_NUM_MAX]; // 最大值
    uchar alarm[LINE_NUM_MAX]; // 告警值 0表示未告警  1表示已告警 2表示已记录

    uint crMin[LINE_NUM_MAX]; // 临界最小值
    uint crMax[LINE_NUM_MAX]; // 临界最大值
    uchar crAlarm[LINE_NUM_MAX]; // 临界告警值
}sDataPowUnit;

/**
 * 插接位数据对象：包括电流，电压，功率，电能，开关状态，插接位名称
 */
typedef struct _sObjData {
    int lineNum; //相数
    sDataUnit vol; // 电压
    sDataUnit cur;  // 电流

    sDataPowUnit pow; // 功率
    uint ele[LINE_NUM_MAX]; // 电能
    ushort pf[LINE_NUM_MAX]; // 功率因素
    uchar sw[LINE_NUM_MAX]; // 开关状态
    uint apPow[LINE_NUM_MAX]; // 视在功率
    ushort ratedCur[LINE_NUM_MAX]; // 额定电流
    ushort wave[LINE_NUM_MAX]; // 谐波值
    uint reactivePower[LINE_NUM_MAX]; // 无功功率

    ushort pl[3]; // 负载百分比
    ushort curThd[3]; // 电流谐波含量
    ushort volThd[3]; // 电压谐波含量
    uint totalPow;
}sObjData;


/**
 * 环境数据结构体
 */
typedef struct _sEnvData {
    sDataUnit tem; // 温度
//    sDataUnit hum; // 湿度
}sEnvData;


/**
 * 插接箱数据结构体：包括最多三个插接位，插接箱名称
 */
typedef struct _sBoxData {
    char offLine; // 离线标识
    char loopNum; // 回路数量
    char version;

    sObjData data; // 回路数据
    char loopName[LINE_NUM_MAX][NAME_LEN]; // 回路名称
    sEnvData env; // 环境状态

    sTgObjData tgBox; // 插接箱统计信息
    sLineTgObjData lineTgBox;
    char boxAlarm, boxVolAlarm, boxCurAlarm, boxEnvAlarm , boxPowerAlarm; // 插接箱告警
    char boxStatus; // 插接箱状态
    char boxSpec; //  0 表示 SI  1 表示 IP
    char boxName[NAME_LEN]; // 插接箱名称

    int ratedCur; // 额定电流
    int rate; // 电压频率
    int minRate;// 电压频率最小值
    int maxRate;// 电压频率最大值
    char dc; // 交直流标志位
    uchar lps; // 防雷开关
    uchar lpsAlarm;//防雷开关告警值 0表示未告警 1表示已告警 2表示已记录
    uchar HzAlarm;//频率告警值 0表示未告警 1表示已告警 2表示已记录

    ushort rtuLen;
    uchar rtuArray[SRC_DATA_LEN_MAX];
}sBoxData;


typedef struct _sThdData {
    ushort curThd[3][35]; // 电流谐波含量
    ushort volThd[3][35]; // 电压谐波含量
}sThdData;


/**
 * 母线数据结构体：包括插接箱数据，电压频率 母线名称
 */
typedef struct _sBusData{
    int boxNum; // 插接箱数量
    sBoxData   box[BOX_NUM+1];  // 最多20个插接箱
    char busName[NAME_LEN]; // 母线名称
    sThdData thdData;
}sBusData;

typedef struct _sDataPacket
{
    sBusData data[BUS_NUM];  // 四条母线数据
    bool initFlag;
}sDataPacket;



/**
 * 设置地址枚举：包括各种参数的地址
 */
//enum  sSetType{
//     SetAddress        = 0x1001           //地址
//    ,VoltageMAX_L1     = 0x1002           //电压上限
//    ,VoltageMIN_L1     = 0x1003           //电压下限
//    ,VoltageMAX_L2     = 0x1004
//    ,VoltageMIN_L2     = 0x1005
//    ,VoltageMAX_L3     = 0x1006
//    ,VoltageMIN_L3     = 0x1007
//    ,VoltageMAX_L4     = 0x1008
//    ,VoltageMIN_L4     = 0x1009
//    ,VoltageMAX_L5     = 0x100A
//    ,VoltageMIN_L5     = 0x100B
//    ,VoltageMAX_L6     = 0x100C
//    ,VoltageMIN_L6     = 0x100D
//    ,VoltageMAX_L7     = 0x100E
//    ,VoltageMIN_L7     = 0x100F
//    ,VoltageMAX_L8     = 0x1010
//    ,VoltageMIN_L8     = 0x1011
//    ,VoltageMAX_L9     = 0x1012
//    ,VoltageMIN_L9     = 0x1013
//    ,CurrentMAX_L1     = 0x1020           //电流上限
//    ,CurrentMIN_L1     = 0x1021           //电流下限
//    ,CurrentMAX_L2     = 0x1022
//    ,CurrentMIN_L2     = 0x1023
//    ,CurrentMAX_L3     = 0x1024
//    ,CurrentMIN_L3     = 0x1025
//    ,CurrentMAX_L4     = 0x1026
//    ,CurrentMIN_L4     = 0x1027
//    ,CurrentMAX_L5     = 0x1028
//    ,CurrentMIN_L5     = 0x1029
//    ,CurrentMAX_L6     = 0x102A
//    ,CurrentMIN_L6     = 0x102B
//    ,CurrentMAX_L7     = 0x102C
//    ,CurrentMIN_L7     = 0x102D
//    ,CurrentMAX_L8     = 0x102E
//    ,CurrentMIN_L8     = 0x102F
//    ,CurrentMAX_L9     = 0x1030
//    ,CurrentMIN_L9     = 0x1031
//    ,temperatureMAX_1  = 0x1032           //温度上限
//    ,temperatureMIN_1  = 0x1033           //温度下限
//    ,temperatureMAX_2  = 0x1034
//    ,temperatureMIN_2  = 0x1035
//    ,temperatureMAX_3  = 0x1036
//    ,temperatureMIN_3  = 0x1037
//    ,BaudRate          = 0x1038           //00:9600 01:4800 02:9600 03:19200 04:38400
//    ,ClearEle          = 0x1039
//    ,SetStartBoxAddr   = 0x1040           //设置始端箱地址1-31
//    ,SetBuzzer         = 0x1041           //1:开启 0:关闭
//    ,SetBreaker        = 0x1042           //断路器开关状态
//    ,PowerMAX_L1       = 0x1050           //功率上限
//    ,PowerMIN_L1       = 0x1051           //功率下限
//    ,PowerMAX_L2       = 0x1052
//    ,PowerMIN_L2       = 0x1053
//    ,PowerMAX_L3       = 0x1054
//    ,PowerMIN_L3       = 0x1055
//    ,PowerMAX_L4       = 0x1056
//    ,PowerMIN_L4       = 0x1057
//    ,PowerMAX_L5       = 0x1058
//    ,PowerMIN_L5       = 0x1059
//    ,PowerMAX_L6       = 0x105A
//    ,PowerMIN_L6       = 0x105B
//    ,PowerMAX_L7       = 0x105C
//    ,PowerMIN_L7       = 0x105D
//    ,PowerMAX_L8       = 0x105E
//    ,PowerMIN_L8       = 0x105F
//    ,PowerMAX_L9       = 0x1060
//    ,PowerMIN_L9       = 0x1061
//    ,SetHzMAX          = 0x1062
//    ,SetHzMIN          = 0x1063
//};

enum  sSetType{
     SetAddress        = 0x1001           //地址
    ,VoltageMAX_L1      = 0x1002           //电压上限
    ,VoltageMIN_L1      = 0x1003           //电压下限
    ,VoltageMAX_L2      = 0x1004
    ,VoltageMIN_L2      = 0x1005
    ,VoltageMAX_L3      = 0x1006
    ,VoltageMIN_L3      = 0x1007
    ,VoltageMAX_L4      = 0x1008
    ,VoltageMIN_L4      = 0x1009
    ,VoltageMAX_L5      = 0x100A
    ,VoltageMIN_L5      = 0x100B
    ,VoltageMAX_L6      = 0x100C
    ,VoltageMIN_L6      = 0x100D
    ,VoltageMAX_L7      = 0x100E
    ,VoltageMIN_L7      = 0x100F
    ,VoltageMAX_L8      = 0x1010
    ,VoltageMIN_L8      = 0x1011
    ,VoltageMAX_L9      = 0x1012
    ,VoltageMIN_L9      = 0x1013

    ,CurrentMAX_L1      = 0x1014           //电流上限
    ,CurrentMIN_L1      = 0x1015           //电流下限
    ,CurrentMAX_L2      = 0x1016
    ,CurrentMIN_L2      = 0x1017
    ,CurrentMAX_L3      = 0x1018
    ,CurrentMIN_L3      = 0x1019
    ,CurrentMAX_L4      = 0x101A
    ,CurrentMIN_L4      = 0x101B
    ,CurrentMAX_L5      = 0x101C
    ,CurrentMIN_L5      = 0x101D
    ,CurrentMAX_L6      = 0x101E
    ,CurrentMIN_L6      = 0x101F
    ,CurrentMAX_L7      = 0x1020
    ,CurrentMIN_L7      = 0x1021
    ,CurrentMAX_L8      = 0x1022
    ,CurrentMIN_L8      = 0x1023
    ,CurrentMAX_L9      = 0x1024
    ,CurrentMIN_L9      = 0x1025


    ,PowerMAX_L1        = 0x1026           //功率上限
    ,PowerMIN_L1        = 0x1027          //功率下限
    ,PowerMAX_L2        = 0x1028
    ,PowerMIN_L2        = 0x1029
    ,PowerMAX_L3        = 0x102A
    ,PowerMIN_L3        = 0x102B
    ,PowerMAX_L4        = 0x102C
    ,PowerMIN_L4        = 0x102D
    ,PowerMAX_L5        = 0x102E
    ,PowerMIN_L5        = 0x102F
    ,PowerMAX_L6        = 0x1030
    ,PowerMIN_L6        = 0x1031
    ,PowerMAX_L7        = 0x1032
    ,PowerMIN_L7        = 0x1033
    ,PowerMAX_L8        = 0x1034
    ,PowerMIN_L8        = 0x1035
    ,PowerMAX_L9        = 0x1036
    ,PowerMIN_L9        = 0x1037

    ,SetHzMAX           = 0x1038
    ,SetHzMIN           = 0x1039

    ,temperatureMAX_1   = 0x103A           //温度上限
    ,temperatureMIN_1   = 0x103B           //温度下限
    ,temperatureMAX_2   = 0x103C
    ,temperatureMIN_2   = 0x103D
    ,temperatureMAX_3   = 0x103E
    ,temperatureMIN_3   = 0x103F
    ,temperatureMAX_4   = 0x1040
    ,temperatureMIN_4   = 0x1041
    ,BaudRate           = 0x1077           //00:9600 01:4800 02:9600 03:19200 04:38400
    ,ClearEle           = 0x1078
    ,SetStartBoxAddr    = 0x1079           //设置始端箱地址1-31
    ,SetBuzzer          = 0x1080           //1:开启 0:关闭
    ,SetBreaker         = 0x1081           //断路器开关状态
};

sDataPacket *share_mem_get();
void share_mem_init();
void share_mem_free();
void share_mem_del();

#endif // DATAPACKET_H
