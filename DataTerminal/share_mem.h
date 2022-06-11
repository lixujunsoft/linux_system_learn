#ifndef _SHARE_MEM_H_INCLUDED_
#define _SHARE_MEM_H_INCLUDED_

#include "ngx_typedef.h"
#include "ngx_core.h"

typedef struct {
    char pathName[128];
    int projId;
    size_t size;
    int shmflg;

    int shmid;
    void *shmaddr;
} ShmInfo;

ShmInfo *ShmInfoConstruct(char *pathName, int projId, size_t size, int shmflg);
int ShmInit(ShmInfo *shmInfo);
int ShmDestroy(ShmInfo *shmInfo);

#endif