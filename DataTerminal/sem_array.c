#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

#include "sem_array.h"
#include "ngx_typedef.h"
#include "ngx_core.h"

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
} un;

int SemArray_P(SemArrayInfo *semArrayInfo, int num)
{
    struct sembuf buf = {num, -1, 0};
    if (semop(semArrayInfo->semid, &buf, 1) == -1) {
        perror("SemArray_P");
        return RET_ERROR;
    }
}

int SemArray_V(SemArrayInfo *semArrayInfo, int num)
{
    struct sembuf buf = {num, 1, 0};
    if (semop(semArrayInfo->semid, &buf, 1) == -1) {
        perror("SemArray_V");
        return RET_ERROR;
    }
}

static int initSem(int semid, int s[], int n)
{
    int i;
    union semun semunTmp;
    for (i = 0; i < n; i++) {
        semunTmp.val = s[i];
        if (semctl(semid, i, SETVAL, semunTmp) == -1) {
            perror("initSem");
            return RET_ERROR;
        }
    }

    return RET_OK;
}

int SemArrayInit(SemArrayInfo *semInfo)
{
    key_t key;
    int semid;

    if (NULL == semInfo) {
        return RET_ERROR;
    }

    if ((key = ftok(semInfo->pathName, semInfo->projId)) == -1) {
        return RET_ERROR;
    }

    semid = semget(key, semInfo->nsems, semInfo->semflg);
    if (-1 == semid) {
        if (errno != EEXIST) {
            return RET_ERROR;
        }
        semid = semget(key, 0, 0666);
    }

    if(initSem(semid, semInfo->semArrayInitValue, semInfo->nsems) == -1) {
        return RET_ERROR;
    }

    semInfo->semid = semid;
    return RET_OK;
}

int SemArrayDestroy(SemArrayInfo *semInfo)
{
    if (semctl(semInfo->semid, 0, IPC_RMID, NULL) == -1) { //删除信号灯集
        return RET_ERROR; 
    }

    free(semInfo);
    return RET_OK;
}

SemArrayInfo *SemInfoConstruct(char *pathName, int projId, int nsems, int semflg)
{
    SemArrayInfo *semArrayInfo = (SemArrayInfo*)malloc(sizeof(SemArrayInfo));
    if (NULL == semArrayInfo) {
        return NULL;
    }

    memset(semArrayInfo, 0, sizeof(SemArrayInfo));
    strncpy(semArrayInfo->pathName, pathName, strlen(pathName));
    semArrayInfo->projId = projId;
    semArrayInfo->nsems = nsems;
    semArrayInfo->semflg = semflg;

    semArrayInfo->semid = -1;
    return semArrayInfo;
}