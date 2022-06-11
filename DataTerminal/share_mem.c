#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "share_mem.h"
#include "ngx_typedef.h"
#include "ngx_core.h"

int ShmInit(ShmInfo *shmInfo)
{
    key_t key;
    int shmid;
    void *shmaddr = NULL;
    
    if (NULL == shmInfo) {
        return RET_ERROR;
    }

    if ((key = ftok(shmInfo->pathName, shmInfo->projId)) == -1) {
        perror("ftok");
        return RET_ERROR;
    }

    if ((shmid = shmget(key, shmInfo->size, shmInfo->shmflg)) == -1) { //得到共享内存的id
        return RET_ERROR;
    }

    if ((shmaddr = shmat(shmid, NULL, 0)) == (void*)-1) { //得到共享内存映射后的起始地址
        return RET_ERROR;
    }

    shmInfo->shmid = shmid;
    shmInfo->shmaddr = shmaddr;
    return RET_OK;
}

int ShmDestroy(ShmInfo *shmInfo)
{
    struct shmid_ds buf;

    if (NULL == shmInfo->shmaddr) {
        return RET_OK;
    }

    if (shmdt(shmInfo->shmaddr) == -1) { // 解除该进程的共享内存映射
        return RET_ERROR;
    }

    if (shmctl(shmInfo->shmid, IPC_RMID, NULL) == -1) { //删除共享内存
        return RET_ERROR;
    }

    free(shmInfo);
    return RET_OK;
}

ShmInfo *ShmInfoConstruct(char *pathName, int projId, size_t size, int shmflg)
{
    ShmInfo *shmInfo = (ShmInfo*)malloc(sizeof(ShmInfo));
    if (NULL == shmInfo) {
        return NULL;
    }
    memset(shmInfo, 0, sizeof(ShmInfo));
    strncpy(shmInfo->pathName, pathName, strlen(pathName));
    shmInfo->projId = projId;
    shmInfo->size = size;
    shmInfo->shmflg = shmflg;

    shmInfo->shmid = -1;
    shmInfo->shmaddr = NULL;

    return shmInfo;
}