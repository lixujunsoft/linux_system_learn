#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <ctype.h>
#include "wrap.h"

#define MAXLINE 80
#define SERV_PORT 8000
#define OPEN_MAX 1024

int main(int argc, char *argv[])
{
    int i, j, listenfd, connfd, sockfd;
    int nready, efd, res;
    ssize_t n;
    char buf[MAXLINE], str[INET_ADDRSTRLEN];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    struct epoll_event tep, ep[OPEN_MAX];

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    Listen(listenfd, 20);

    efd = epoll_create(OPEN_MAX);
    if (efd == -1) {
        perr_exit("epoll_create");
    }

    tep.events = EPOLLIN;
    tep.data.fd = listenfd;
    res = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &tep);
    if (res == -1) {
        perr_exit("epoll_ctl");
    }

    for (;;) {
        nready = epoll_wait(efd, ep, OPEN_MAX, -1);   // 阻塞监听
        if (nready == -1) {
            perr_exit("epoll_wait");
        }
        for (i = 0; i < nready; i++) {
            if (!(ep[i].events & EPOLLIN)) {
                continue;
            }
            if (ep[i].data.fd == listenfd) {
                clilen = sizeof(cliaddr);
                connfd = Accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
                printf("received from %s at port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));

                if (j == OPEN_MAX) {
                    perr_exit("too many clients");
                }

                tep.events = EPOLLIN;
                tep.data.fd = connfd;
                res = epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &tep);
                if (res == -1) {
                    perr_exit("epoll_ctl");
                }
            } else {
                sockfd = ep[i].data.fd;
                n = Read(sockfd, buf, MAXLINE);
                if (n == 0) {
                    res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);
                    if (res == -1) {
                        perr_exit("epoll_ctl");
                    }
                    Close(sockfd);
                    printf("client[%d] closed connection\n", j);
                } else {
                    for (j = 0; j < n; j++) {
                        buf[j] = toupper(buf[j]);
                    }
                    Write(sockfd, buf, n);  // yyyyyYYY
                }
            }
        }
    }
}