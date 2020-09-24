/*
 * datapacket.h
 * 共享内存操作接口函数实现
 *
 *  Created on: 2017年5月24日
 *      Author: Lzy
 */
#include "datapacket.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY	1234

/**
 * 获取共享内存
 */

sDataPacket gDataPacket;

sDataPacket *share_mem_get()
{
//    int shmid = shmget((key_t)SHM_KEY, sizeof(sDataPacket), 0666|IPC_CREAT);    //创建共享内存
//    if(shmid == -1) {
//        fprintf(stderr, "shmget failed\n");
//        return NULL;
//    }

//    //将共享内存连接到当前进程的地址空间
//    void *shm = shmat(shmid, 0, 0);
//    if(shm == (void*)-1) {
//        fprintf(stderr, "shmat failed\n");
//        return NULL;
//    }
////	printf("\nMemory attached at %X\n", (int)shm);
//    return (sDataPacket *)shm;
    return &gDataPacket;
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
        printf("\nMemory init ok!!!\n");
    }
}

/**
 * 把共享内存从当前进程中分离
 */
void share_mem_free()
{
    sDataPacket *shm = share_mem_get();
//    if(shmdt(shm) == -1) {
//        fprintf(stderr, "shmdt failed\n");
//        exit(EXIT_FAILURE);
//    }
}

/**
 * 删除共享内存
 */
void share_mem_del()
{
    //int shmid = shmget((key_t)SHM_KEY, sizeof(sDataPacket), 0666|IPC_CREAT);    //创建共享内存
//    int shmid = -1;    //创建共享内存
//    if(shmid >= 0) {
//        if(shmctl(shmid, IPC_RMID, 0) == -1) //删除共享内存
//            fprintf(stderr, "shmctl(IPC_RMID) failed\n");
//    }
}
