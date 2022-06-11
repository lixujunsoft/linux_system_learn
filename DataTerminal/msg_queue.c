#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include "msg_queue.h"
#include "ngx_typedef.h"
#include "ngx_core.h"

#define ARRAY_SIZE      2
#define UNDEFINED_MSGID -1

int MsgidArray[MsgQueueIndexArray_MAX] = {UNDEFINED_MSGID};

int MsgQueueInit(MsgQueueIndexEnum msgQueueIndex)
{
    key_t key;
    
    if (UNDEFINED_MSGID != MsgidArray[msgQueueIndex]) {
        return RET_OK;
    }

    key = ftok("/tmp", msgQueueIndex);
    int msgid = msgget(key, IPC_CREAT | 0777);
    if (RET_ERROR == msgid) {
        return RET_ERROR;
    }
    MsgidArray[msgQueueIndex] = msgid;

    return RET_OK;
}

int MsgSend(MsgQueueMsg *msg, MsgQueueIndexEnum msgQueueIndex)
{
    if (UNDEFINED_MSGID == MsgidArray[msgQueueIndex]) {
        return RET_ERROR;
    }
    
    int error = msgsnd(MsgidArray[msgQueueIndex], msg, sizeof(msg->mtext), 0);
    if (RET_ERROR == error) {
        return RET_ERROR;
    }
    return RET_OK;
}

int MsgRecv(MsgQueueMsg *msg, MsgQueueIndexEnum msgQueueIndex)
{
    if (UNDEFINED_MSGID == MsgidArray[msgQueueIndex]) {
        return RET_ERROR;
    }
    
    int error = msgsnd(MsgidArray[msgQueueIndex], msg, sizeof(msg->mtext), 0);
    if (RET_ERROR == error) {
        exit(1);
    }
    return RET_OK;
}

void MsgQueueDestroy()
{
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (UNDEFINED_MSGID == MsgidArray[i]) {
            continue;
        }

        msgctl(MsgidArray[i], IPC_RMID, NULL);
        MsgidArray[i] = UNDEFINED_MSGID;
    }
}