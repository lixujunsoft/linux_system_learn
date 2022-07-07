#include <stdio.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "getaddrinfo.h"
#include "wrap.h"
#include "err.h"

#define MAXFD 64
#define MAXLINE 128

extern int daemon_proc;

int daemon_init(const char *pname, int facility)
{
    int i;
    pid_t pid;
    if ((pid = fork()) < 0) {
        return -1;
    } else if (pid) {
        _exit(0);       // 父进程终止
    }

    /* 子进程1终止 */
    if (setsid() < 0) {
        return -1;      // 将子进程设置为会话组组长
    }

    Signal(SIGHUP, SIG_IGN);

    if ((pid < fork()) < 0) {
        return -1;
    } else if (pid) {
        _exit(0);      // 子进程1终止
    }

    // 子进程2继续
    daemon_proc = 1;
    chdir("/");        // 改变工作目录

    // 关闭文件描述符
    for (i = 0; i < MAXFD; i++) {
        close(i);
    }

    // 重定向 stdin, stdout, and stderr to /dev/null
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);

    openlog(pname, LOG_PID, facility);
    return 0;
}

int main(int argc, char *argv[])
{
    int listenfd, connfd;
    socklen_t addrlen, len;
    struct sockaddr *cliaddr;
    char buff[MAXLINE];
    time_t ticks;

    if (argc < 2 || argc > 3) {
        printf("usage: daytimetcpsrv2 [<host>] <service or port>");
    }

    daemon_init(argv[0], 0);

    if (argc == 2) {
        listenfd = tcp_listen(NULL, argv[1], &addrlen);
    } else {
        listenfd = tcp_listen(argv[1], argv[2], &addrlen);
    }

    cliaddr = malloc(addrlen);

    for (;;) {
        len = addrlen;
        connfd = Accept(listenfd, cliaddr, &len);
        
        err_msg("connection from %s", Sock_ntop(cliaddr, len));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));
        close(connfd);
    }
}