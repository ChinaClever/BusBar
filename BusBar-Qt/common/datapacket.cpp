/*
 * datapacket.h
 * 共享内存操作接口函数实现
 *
 *  Created on: 2017年5月24日
 *      Author: Lzy
 */
#include "datapacket.h"
#include <sys/types.h>

#define SHM_KEY	1234

/**
 * 获取共享内存
 */

sDataPacket *share_mem_get()
{
    static sDataPacket *ptr = nullptr;
    if(!ptr) {
        ptr = new sDataPacket();
    }

    return ptr;
}

/**
 * 共享内存初始化
 */
void share_mem_init()
{
    sDataPacket *shared = share_mem_get();//指向shm
    if(shared) {
        memset(shared, 0, sizeof(sDataPacket));
        for(int i=0; i<BUS_NUM; ++i) {
            shared->data[i].box[0].dc = 1;
        }
    }
}

/**
 * 把共享内存从当前进程中分离
 */
void share_mem_free()
{
    sDataPacket *shm = share_mem_get();
    delete shm;
}

/**
 * 删除共享内存
 */
void share_mem_del()
{
   share_mem_free();
}
