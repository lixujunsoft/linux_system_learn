#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include "ngx_typedef.h"
#include "ngx_core.h"

int MsgQueueInit(MsgQueueInfo *msgQueueInfo)
{
    key_t key;
    int msgid;

    if (NULL == msgQueueInfo) {
        return RET_ERROR;
    }

    if ((key = ftok(msgQueueInfo->pathName, msgQueueInfo->projId)) == -1) {
        perror("ftok");
        return RET_ERROR;
    }
    
    msgid = msgget(key, msgQueueInfo->msgflg);
    if (RET_ERROR == msgid) {
        return RET_ERROR;
    }
    msgQueueInfo->msgid = msgid;

    return RET_OK;
}

int MsgSend(MsgQueueInfo *msgQueueInfo, MsgQueueMsg *msg)
{
    if (NULL == msgQueueInfo || NULL == msg) {
        return RET_ERROR;
    }

    int error = msgsnd(msgQueueInfo->msgid, msg, sizeof(msg->mtext), 0);
    if (RET_ERROR == error) {
        perror("MsgSend");
        return RET_ERROR;
    }
    return RET_OK;
}

int MsgRecv(MsgQueueInfo *msgQueueInfo, MsgQueueMsg *msg)
{
    if (NULL == msgQueueInfo || NULL == msg) {
        return RET_ERROR;
    }
    
    int error = msgrcv(msgQueueInfo->msgid, msg, sizeof(msg->mtext), msg->mtype, 0);
    if (RET_ERROR == error) {
        perror("MsgRecv");
        return RET_ERROR;
    }
    return RET_OK;
}

int MsgQueueDestroy(MsgQueueInfo *msgQueueInfo)
{
    if (NULL == msgQueueInfo) {
        return RET_OK;
    }

    if (msgctl(msgQueueInfo->msgid, IPC_RMID, NULL) == -1) {
        return RET_ERROR;
    }
    return RET_OK;
}

MsgQueueInfo *MsgQueueInfoConstruct(char *pathName, int projId, int msgflg)
{
    MsgQueueInfo *msgQueueInfo = (MsgQueueInfo*)malloc(sizeof(MsgQueueInfo));
    if (NULL == msgQueueInfo) {
        return NULL;
    }

    memset(msgQueueInfo, 0, sizeof(msgQueueInfo));
    strncpy(msgQueueInfo->pathName, pathName, sizeof(pathName));
    msgQueueInfo->projId = projId;
    msgQueueInfo->msgflg = msgflg;

    return msgQueueInfo;
}