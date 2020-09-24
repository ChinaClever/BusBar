/*
 * pduhashdevinfosave.cpp
 * PDU设备信息的保存
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pduhashdevinfosave.h"



/**
 * @brief 保存设备工作模式
 * @param type
 * @param data
 */
static void pdu_devMode_save(PduDevType *type, pdu_dev_data *data)
{
    int mode = data->data[0]; /*主从模式*/
    if(mode >= 0)
        type->ms = mode;
}

/**
 * @brief 设备信息保存
 * @param info
 * @param data
 */
static void pdu_hashDevType_save(PduDevType *type,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f;
    switch (fc)
    {
    case PDU_CMD_DEV_NAME: // 设备工作名称
        pdu_devStr_save(type->name, data);
        break;

    case PDU_CMD_DEV_MODE: // 设备工作模式
        pdu_devMode_save(type, data);
        break;

    case PDU_CMD_DEV_TYPE: // 设备型号
        pdu_devStr_save(type->typeStr, data);
        break;

    }
}

/**
 * @brief PDU设备地址
 * @param addr
 * @param data
 */
static void pdu_hashDevAddr_save(PduDevAddr *addr,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f;
    switch (fc)
    {
    case PDU_CMD_DEV_AREA: // 区
        pdu_devStr_save(addr->area, data);
        break;

    case PDU_CMD_DEV_GROUP: // 组
        pdu_devStr_save(addr->group, data);
        break;

    case PDU_CMD_DEV_CAB: //机柜
        pdu_devStr_save(addr->cab, data);
        break;
    }
}

/**
 * @brief 设备系统信息
 * @param sys
 * @param data
 */
static void pdu_hashDevSys_save(PduDevSys *sys,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f;
    switch (fc)
    {
    case PDU_CMD_SYS_VER:
        pdu_devStr_save(sys->version, data);
        break;

    case PDU_CMD_SYS_UP:
        pdu_devStr_save(sys->upTime, data);
        break;

    case PDU_CMD_SYS_RUN:
        sys->runTime = data->data[0] *256 + data->data[1];
        break;

    }
}

/**
 * @brief 设备CPU信息
 * @param cpu
 * @param data
 */
static void pdu_hashDevCPU_save(PdudevCPU *cpu,pdu_dev_data *data)
{
    int fc = data->fn[1] & 0x0f;
    switch (fc)
    {
    case PDU_CMD_CPU_TEMP:
        cpu->temp = data->data[0] *256 + data->data[1];
        break;

    case PDU_CMD_CPU_USR:
        cpu->use = data->data[0];
        break;

    case PDU_CMD_CPU_NAME:
        pdu_devStr_save(cpu->name, data);
        break;

    case PDU_CMD_CPU_FRE:
        cpu->frequency = data->data[0] *256 + data->data[1];
        break;

    case PDU_CMD_MEM_NAME:
        pdu_devStr_save(cpu->mem, data);
        break;

    case PDU_CMD_MEM_FRE:
        cpu->memFre = data->data[0] *256 + data->data[1];
        break;

    case PDU_CMD_MEM_SIZE:
        cpu->memSize = data->data[0] *256 + data->data[1];
        break;
    }
}




/**
 * @brief 设备信息保存
 * @param info
 * @param data
 */
void pdu_hashDevInfo_save(PduDevInfo *info,pdu_dev_data *data)
{
    int fc = data->fn[1] >> 4;
    switch (fc)
    {
    case PDU_CMD_DEVTYPE: // 设备类型
        pdu_hashDevType_save(info->type, data);
        break;

    case PDU_CMD_DEVADDR:
        pdu_hashDevAddr_save(info->addr, data);
        break;

    case PDU_CMD_DEVSYS:
        pdu_hashDevSys_save(info->type->sys, data);
        break;

    case PDU_CMD_DEVCPU:
        pdu_hashDevCPU_save(info->type->cpu, data);
        break;

    default:
        qDebug() << "pdu_hashDevInfo_save err";
        break;
    }

}



