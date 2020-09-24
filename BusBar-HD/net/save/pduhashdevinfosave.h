#ifndef PDUHASHDEVINFOSAVE_H
#define PDUHASHDEVINFOSAVE_H
#include "pduhashcom.h"

typedef enum {
    PDU_CMD_DEV_NAME = 1, // 设备名称
    PDU_CMD_DEV_MODE, //设备工作模式
    PDU_CMD_DEV_TYPE, //设备型号
}PDU_CMD_DEV_TYPE_;


typedef enum {
    PDU_CMD_DEV_AREA = 1,
    PDU_CMD_DEV_GROUP,
    PDU_CMD_DEV_CAB
}PDU_CMD_DEV_ADDR_;


typedef enum {
    PDU_CMD_SYS_VER = 1,
    PDU_CMD_SYS_UP,
    PDU_CMD_SYS_RUN
}PDU_CMD_DEV_SYS_;

typedef enum {
    PDU_CMD_CPU_TEMP = 1,
    PDU_CMD_CPU_USR,
    PDU_CMD_CPU_NAME,
    PDU_CMD_CPU_FRE,

    PDU_CMD_MEM_NAME,
    PDU_CMD_MEM_FRE,
    PDU_CMD_MEM_SIZE
}PDU_CMD_DEV_CPDU_;

typedef enum {
    PDU_CMD_DEVINFO = 5, //设备信息 主功能码为5

    PDU_CMD_DEVTYPE=1,
    PDU_CMD_DEVADDR,
    PDU_CMD_DEVSYS,
    PDU_CMD_DEVCPU
}PDU_CMD_DEV_INFO;

void pdu_hashDevInfo_save(PduDevInfo *info,pdu_dev_data *data);

#endif // PDUHASHDEVINFOSAVE_H
