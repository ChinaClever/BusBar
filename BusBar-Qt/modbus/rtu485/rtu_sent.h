#ifndef RTU_SENT_H
#define RTU_SENT_H

#include <QtCore>

#define RTU_LINE_NUM 9 // 3相
#define RTU_TH_NUM 3 // 3个传感器
//1.4版本
//#define RTU_SENT_LEN (22*RTU_LINE_NUM+1+3*3+1+1+3+11+6)
//1.3版本
#define RTU_SENT_LEN (22*RTU_LINE_NUM+1+3*3+1+1+3+11+6)  // 长度需要改变 ////============ 加上开关，功率因素之后，是为14 [追加交直流区分]
#define RTU_SENT_DC_LEN (22*4+1+3*3+1+1+3+5)  // 长度需要改变 直流长度 [追加交直流区分

struct Rtu_Sent {
    Rtu_Sent():fn(3),reg(0),len(RTU_SENT_LEN){} // 下位机有问题
    uchar addr; // 表示从机地址码
    uchar fn;  // 表示功能码
    ushort reg; // 表示寄存器首地址
    ushort len; // 表示数据长度
    ushort crc; // 表示CRC校验
};

ushort rtu_crc(uchar *buf, int len);
int rtu_sent_buff(uchar addr, uchar *buf);
int rtu_sent_buff(uchar addr, ushort reg, ushort len, uchar *buf);

#endif // RTU_SENT_H
