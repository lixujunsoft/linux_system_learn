#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "wrap.h"
#include <errno.h>
#include <signal.h>

#define SERV_PORT 8000
#define MAXLINE 128

/*
1.当fork子进程时，必须捕获SIGCHLD信号
2.当捕获信号时，必须处理被中断的系统调用
3.SIGCHLD的信号处理函数必须正确编写，应使用waitpid函数以免留下僵尸进程
*/
int main(int argc, char *argv[])
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    Signal(SIGCHLD, sig_chld);
    
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    Listen(listenfd, 10);

    for (;;) {
        clilen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen)) < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                perr_exit("accept error");
            }
        }

        if ((childpid = fork()) == 0) {
            Close(listenfd);
            str_echo(connfd);
            // sleep(10);  // 用于测试(服务器)CLOSE_WAIT、(客户端)FIN_WAIT_2
            exit(0);
        }
        Close(connfd);
    }
}