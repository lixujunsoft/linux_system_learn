#ifndef _MSG_QUEUE_H_INCLUDED_
#define _MSG_QUEUE_H_INCLUDED_

#include "ngx_typedef.h"
#include "ngx_core.h"

typedef struct
{
    long mtype;
    char mtext[1024];
} MsgQueueMsg;

typedef enum {
    DEBUG_LOG_MSG,
    COMMUNICATION_msg,
    MsgQueueIndexArray_MAX
} MsgQueueIndexEnum;

int MsgQueueInit(MsgQueueIndexEnum msgQueueIndex);
int MsgSend(MsgQueueMsg *msg, MsgQueueIndexEnum msgQueueIndex);
int MsgRecv(MsgQueueMsg *msg, MsgQueueIndexEnum msgQueueIndex);
void MsgQueueDestroy();

#endif