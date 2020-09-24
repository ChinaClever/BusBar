#ifndef PDUHASHSAVECOM_H
#define PDUHASHSAVECOM_H

#include "netdataformat.h"
#include "common.h"

#define NET_DATA_VERSION		2 /*IP设备通讯类型*/
#define OFF_LINE_TIME   35

typedef enum {
    PDU_CMD_1=1,
    PDU_CMD_2,
    PDU_CMD_3,
    PDU_CMD_4,
    PDU_CMD_5,
    PDU_CMD_6,
    PDU_CMD_7,
    PDU_CMD_8,
    PDU_CMD_9,
    PDU_CMD_10,
    PDU_CMD_11,
    PDU_CMD_12,
    PDU_CMD_13,
    PDU_CMD_14,
    PDU_CMD_15,
    PDU_CMD_16,
}PDU_CMD_NUM;

/*网络通讯类型*/
typedef enum {
    NET_UDP_TYPE=1, //UDP通讯类型
    NET_TCP_TYPE, //TCP通讯类型
}NET_TRA_TYPE;


int get_pdu_devCode(uchar *prt);
bool pdu_netData_check(uchar type, uchar trans);  //协议匹配

int pdu_saveHash_toData(uint *ptr, ushort len, uchar *data, int sizeBit);
int pdu_saveHash_toData(ushort *ptr, ushort len, uchar *data, int sizeBit);
int pdu_saveHash_toData(uchar *ptr, ushort len, uchar *data, int sizeBit);

bool char_to_string(QString &str, uchar *data, ushort len);
void pdu_saveHash_string(char *strBase, ushort len, uchar *data);
void pdu_devStr_save(char *strBase, pdu_dev_data *data);

#endif // PDUHASHSAVECOM_H
