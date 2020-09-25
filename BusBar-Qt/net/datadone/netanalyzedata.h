#ifndef NETANALYZEDATA_H
#define NETANALYZEDATA_H

#include "netdataformat.h"

uchar net_data_xor(uchar *buf, ushort len);
int net_data_analytic(uchar *buf, ushort len, net_data_packet *msg);
int dev_data_analytic(uchar *buf, ushort len, net_dev_data *pkt);

#endif // NETANALYZEDATA_H
