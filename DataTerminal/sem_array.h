#ifndef _SEM_ARRAY_H_INCLUDED_
#define _SEM_ARRAY_H_INCLUDED_

#include "ngx_typedef.h"
#include "ngx_core.h"

#define SEM_ARRAY_SIZE 10

typedef struct {
    char pathName[128];
    int projId;
    int nsems;
    int semflg;

    int semArrayInitValue[SEM_ARRAY_SIZE];
    int semid;
} SemArrayInfo;

SemArrayInfo *SemInfoConstruct(char *pathName, int projId, int nsems, int semflg);
int SemArrayInit(SemArrayInfo *semInfo);
int SemArrayDestroy(SemArrayInfo *semInfo);
int SemArray_V(SemArrayInfo *semArrayInfo, int num);
int SemArray_P(SemArrayInfo *semArrayInfo, int num);

#endif