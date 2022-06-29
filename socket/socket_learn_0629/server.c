#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "wrap.h"
#include <errno.h>

#define SERV_PORT 8000
#define MAXLINE 128

void str_echo(int connfd)
{
    ssize_t n;
    char buf[MAXLINE];

again:
    while ((n = read(connfd, buf, MAXLINE)) > 0) {
        Writen(connfd, buf, n);
    }

    if (n < 0 && errno == EINTR) {
        goto again;
    } else if (n < 0) {
        perr_exit("str_eoch: read error");
    }
}

int main(int argc, char *argv[])
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    Listen(listenfd, 10);

    for (;;) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
        if ((childpid = fork()) == 0) {
            Close(listenfd);
            str_echo(connfd);
            // sleep(10);  // 用于测试(服务器)CLOSE_WAIT、(客户端)FIN_WAIT_2
            exit(0);
        }
        Close(connfd);
    }
}