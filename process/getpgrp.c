#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

void sig_chld(int signum)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        printf("child %d terminated\n", pid);
    }
}

int main(int argc, char *argv[])
{
    pid_t pid;
    
    // signal(SIGCHLD, sig_chld);
    srand(time(NULL));

    for (int i = 0; i < 10; i++) {
        int seconds = rand() % 30 + 10;
        if ((pid = fork()) < 0) {
            continue;
        } else if (pid == 0) {
            // getpgrp() 获取调用进程的进程组ID
            // getpgid(pid_t pid) 若pid是0,返回调用进程的进程组ID
            // 每个进程组有一个组长进程，组长进程的进程组ID等于其进程ID
            printf("child %d sleep %d seconds, pgid = %d\n", getpid(), seconds, getpgrp());
            if (i == 9) {
                setsid();   // 如果调用此函数的进程不是一个进程组的组长，则此函数创建一个新会话
                            // 该进程变成新会话首进程（会话首进程是创建该会话的进程，此时，该进程是新会话中的唯一进程）
                            // 该进程成为一个新进程组的组长进程。新进程组ID是该调用进程的进程ID
                            // 该进程没有控制终端，如果在调用setsid之前该进程有一个控制终端，那么这种联系也被切断

            }
            sleep(seconds);
            exit(0);
        }
    }

    while (1);
    return 0;
}