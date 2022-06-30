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
#include <poll.h>
#include <limits.h>

int main(int argc, char *argv[])
{
    int i, maxi, listenfd, connfd, sockfd;
    int nready;

    ssize_t n;
    char buf[MAXLINE];
    socklen_t clilen;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    for (i = 1; i < OPEN_MAX; i++) {
        client[i].fd = -1;
    }
    maxi = 0;

    for (;;) {
        nready = poll(client, maxi + 1, INFTIM);
        if (client[0].revents & POLLRDNORM) { // 普通数据可读
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);

            for (i = 1; i < OPEN_MAX; i++) {
                if (client[i].fd < 0) {
                    client[i].fd = connfd;
                    break;
                }
            }

            if (i == OPEN_MAX) {
                perr_exit("too many clients");
            }

            client[i].events = POLLRDNORM;
            if (i > maxi) {
                maxi = i;
            }

            if (--nready <= 0) {
                continue;
            }
        }

        for (i = 1; i <= maxi; i++) { // check all clients for data
            if ((sockfd = client[i].fd) < 0) {
                continue;
            }

            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ((n = read(sockfd, buf, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        // connection reset by client
                        Close(sockfd);
                        client[i].fd = -1;
                    } else {
                        perr_exit("read error");
                    }
                } else if (n == 0) {
                    // connection closed by client
                    Close(sockfd);
                    client[i].fd = -1;
                } else {
                    Writen(sockfd, buf, n);
                }

                if (--nready <= 0) {
                    break;          // no more readable descriptors
                }
            }
        }
    }
}