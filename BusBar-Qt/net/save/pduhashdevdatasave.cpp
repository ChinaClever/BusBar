/*
 * pduhashdevdatasave.cpp
 * PDU设备数据保存
 *  主要保存、电流、电压、功率、电能、及环境数据的保存
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduhashdevdatasave.h"


/**
 * @brief 数据单元处理 主要针对 当前值、最大、小值等数据的处理
 * @param unit
 * @param data
 */
static void pdu_hash_unitData(sDataUnit *unit, pdu_dev_data *data)
{
    ushort *ptr = NULL;
    int sizeBit = 2;

    int fc = data->fn[1] & 0x0f; // 处理功能码，第二字节的低四位数据
    switch (fc)
    {
    case PDU_CMD_VALUE:
        ptr = unit->value;
        break;

    case PDU_CMD_MIN:
        ptr = unit->min;
        break;

    case PDU_CMD_MAX:
        ptr = unit->max;
        break;

    case PDU_CMD_ALARM:
        sizeBit = 1;
        // ptr = unit->alarm;
        return; /////====

    case PDU_CDM_CRMIN:
        ptr = unit->crMin;
        break;

    case PDU_CMD_CRMAX:
        ptr = unit->crMax;
        break;

    case PDU_CMD_CRALARM:
        sizeBit = 1;
        // ptr = unit->crAlarm;
        break;

    default:
        qDebug() << "pdu_hashUnitData err" << fc;
        break;
    }

    if(ptr)
        pdu_saveHash_toData(ptr, data->len, data->data, sizeBit);
}


/**
 * @brief 设备对象数据的处理 主要电流、电压、功率、电能
 * @param obj
 * @param data
 */
static void pdu_hash_objData(sObjData *obj,pdu_dev_data *data)
{
    ushort *ptr = NULL;
    uint *iPtr = NULL;
    uchar *cPtr=NULL;
    int sizeBit = 2;

    int fc = data->fn[1] >> 4; // // 处理功能码，第二字节的高四位
    switch (fc)
    {
    case PDU_CMD_NUM_SIZE: // 回路数量
        obj->lineNum  = data->data[0] + (data->data[1]<<8);
        return;

    case PDU_CMD_CUR: // 电流
        pdu_hash_unitData(&(obj->cur), data);
        break;

    case PDU_CMD_VOL: // 电压
        pdu_hash_unitData(&(obj->vol), data);
        break;

    case PDU_CMD_POW: // 功率
        sizeBit = 4;
        iPtr = obj->pow;
        break;

    case PDU_CMD_ELE: // 电能
        sizeBit = 4;
        iPtr = obj->ele;
        break;

    case PDU_CMD_PF: // 功率因素
        ptr = obj->pf;
        break;

    case PDU_CMD_SW: // 开关状态
        sizeBit = 1;
        cPtr = obj->sw;
        break;

    case PDU_CMD_PL:
        ptr = obj->pl;
        break;

    case PDU_CMD_CTHD:
        ptr = obj->curThd;
        break;

    case PDU_CMD_VTHD:
        ptr = obj->volThd;
        break;
    default:
        qDebug() << "pdu_hash_objData err" << fc;
        break;
    }
    if(iPtr) pdu_saveHash_toData(iPtr, data->len, data->data, sizeBit);
    if(ptr)  pdu_saveHash_toData(ptr, data->len, data->data, sizeBit);
    if(cPtr) pdu_saveHash_toData(cPtr, data->len, data->data, sizeBit);
}

/**
 * @brief 环境数据的处理
 * @param env
 * @param data
 */
static void pdu_hash_envData(sEnvData *env,pdu_dev_data *data)
{
    uint *ptr = NULL;
    int sizeBit = 1;

    int fc = data->fn[1] >> 4; // // 处理功能码，第二字节的高四位
    switch (fc)
    {
    case PDU_CMD_TEMP: // 温度
        pdu_hash_unitData(&(env->tem), data);
        break;

    case PDU_CMD_HUM: //湿度
        pdu_hash_unitData(&(env->hum), data);
        break;

//    case PDU_CMD_DOOR: //门禁
//        ptr = env->door;
//        break;

//     case PDU_CMD_WATER: //水浸
//        ptr = env->water;
//        break;

//     case PDU_CMD_SMOKE: //烟雾
//        ptr = env->smoke;
//        break;

     default:
        qDebug() << "pdu_hash_envData err" << fc;
        break;
    }
    if(ptr)
        pdu_saveHash_toData(ptr, data->len, data->data, sizeBit);
}


/**
 * @brief 设置输出位的名称
 * @param name
 * @param data
 */
void pdu_output_name(sBoxData *dev, pdu_dev_data *data)
{
    int l = data->fn[1];
    if(data->len) {
        sprintf(dev->loopName[l], "%s",data->data);
        dev->loopName[l][data->len] = 0;
    }
}


/**
 * @brief 保存设备数据 主要针对PduDevData类操作
 * @param dev
 * @param data
 */
void pdu_hashDevData_save(sBoxData *dev,pdu_dev_data *data)
{
    int fn = data->fn[0]; // 处理功能码第一位数据
    switch (fn)
    {
    case PDU_CMD_LINE: //相电气参数
        pdu_hash_objData(&(dev->data), data);
        dev->loopNum = dev->data.lineNum;
        break;

    case PDU_CMD_OUTPUTNAME: // 输出位电气参数
        pdu_output_name(dev, data);
        break;

    case PDU_CMD_ENV: //环境数据
        pdu_hash_envData(&(dev->env), data);
        break;

    default:
//        qDebug() << "pdu_hashDevData_save err" << fn;
        break;
    }
}



