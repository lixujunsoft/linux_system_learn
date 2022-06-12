#include "ngx_typedef.h"
#include "ngx_core.h"
#include <sys/ipc.h>

#define LOG_MSG_TYPE 99999
#define LOG_THREAD_QUIT "log thread done"

MsgQueueInfo *msgQueueInfo = NULL;
int fd = -1;

MsgQueueMsg logMsg;

int LogInit(char *pathName)
{
    fd = OpenFile(pathName);
    if (RET_ERROR == fd) {
        perror("OpenFile failed");
        return RET_ERROR;
    }

    msgQueueInfo = MsgQueueInfoConstruct("/tmp", fd, IPC_CREAT | 0666);
    if (MsgQueueInit(msgQueueInfo) != RET_OK) {
        perror("MsgQueueInit failed");
        return RET_ERROR;
    }

    memset(&logMsg, 0, sizeof(MsgQueueMsg));
    logMsg.mtype = LOG_MSG_TYPE;

    makethread(WriteLogToFile, &fd);

    return RET_OK;
}

int LogDestroy()
{
    memset(logMsg.mtext, 0, sizeof(logMsg.mtext));
    memmove(logMsg.mtext, LOG_THREAD_QUIT, strlen(LOG_THREAD_QUIT));
    MsgSend(msgQueueInfo, &logMsg);
    sleep(1);
    if (MsgQueueDestroy(msgQueueInfo) != RET_OK) {
        perror("MsgQueueDestroy failed");
        return RET_ERROR;
    }
    return RET_OK;
}

void AddDebugLog(char *debugLogInfo)
{
    if (NULL == debugLogInfo || strlen(debugLogInfo) == 0) {
        return;
    }

    memset(logMsg.mtext, 0, sizeof(logMsg.mtext));
    memmove(logMsg.mtext, debugLogInfo, strlen(debugLogInfo));
    if ('\n' != logMsg.mtext[strlen(logMsg.mtext)]) {
        logMsg.mtext[strlen(debugLogInfo)] = '\n';
    }
    MsgSend(msgQueueInfo, &logMsg);
}

void *WriteLogToFile(void *data)
{
    int fd = *((int*)data);
    while (1) {
        memset(logMsg.mtext, 0, sizeof(logMsg.mtext));
        MsgRecv(msgQueueInfo, &logMsg);
        printf("size = %ld, text = %s",  strlen(logMsg.mtext), logMsg.mtext);
        if (strncmp(logMsg.mtext, LOG_THREAD_QUIT, strlen(LOG_THREAD_QUIT)) == 0) {
            WriteFile(fd, logMsg.mtext, strlen(logMsg.mtext));
            CloseFile(fd);
            break;
        }
        WriteFile(fd, logMsg.mtext, strlen(logMsg.mtext));
    }
}