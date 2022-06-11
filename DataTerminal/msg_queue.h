#ifndef _MSG_QUEUE_H_INCLUDED_
#define _MSG_QUEUE_H_INCLUDED_

#include "ngx_typedef.h"
#include "ngx_core.h"

typedef struct
{
    long mtype;
    char mtext[1024];
} MsgQueueMsg;

typedef struct {
    char pathName[128];
    int projId;
    int msgflg;

    int msgid;
} MsgQueueInfo;

MsgQueueInfo *MsgQueueInfoConstruct(char *pathName, int projId, int msgflg);
int MsgQueueInit(MsgQueueInfo *msgQueueInfo);
int MsgSend(MsgQueueInfo *msgQueueInfo, MsgQueueMsg *msg);
int MsgRecv(MsgQueueInfo *msgQueueInfo, MsgQueueMsg *msg);
int MsgQueueDestroy(MsgQueueInfo *msgQueueInfo);

#endif