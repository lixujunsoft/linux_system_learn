#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MSGKEY 1234

struct msgbuf
{
    long mtype;
    int id;
    char mtext[100];
};

int main()
{
    int msgid;
    int ret;
    struct msgbuf buf;
    pid_t pid;
    key_t key;
    key = ftok(".", 'a');
    printf("%X\n", key);
    msgid = msgget(key, IPC_CREAT | 0777);
    if (-1 == msgid) {
        perror("msgget");
        exit(1);
    }

    pid = fork();
    if (-1 == pid) {
        perror("fork");
        exit(1);
    } else if (0 == pid) {
        while (1) {    // 发消息
            memset(&buf, 0, sizeof(buf));
            scanf("%s", buf.mtext);
            buf.mtype = 3;

            ret = msgsnd(msgid, &buf, sizeof(buf.mtext), 0);
            if (-1 == ret) {
                perror("msgnd");
                exit(1);
            }
            if (!strncmp(buf.mtext, "bye", 3)) {
                buf.mtype = 5;
                ret = msgsnd(msgid, &buf, sizeof(buf.mtext), 0);
                if (-1 == ret) {
                    perror("msgsnd");
                    exit(1);
                }
                break;
            }
        }
    } else {
        while (1) {    // 接收消息
            memset(&buf, 0, sizeof(buf));
            ret = msgrcv(msgid, &buf, sizeof(buf.mtext), 5, 0);
            if (-1 == ret) {
                perror("msgrcv");
                exit(1);
            }
            if (!strncmp(buf.mtext, "bye", 3)) {
                kill(pid, SIGKILL);
                printf("Receive:%s\n", buf.mtext);
                break;
            }
            printf("Receive:%s, %d\n", buf.mtext, buf.id);
        }
        waitpid(pid, NULL, 0);
    }
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}