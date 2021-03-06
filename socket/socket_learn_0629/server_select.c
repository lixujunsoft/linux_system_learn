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
#include <sys/select.h>

int main(int argc, char *argv[])
{
    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];
    
    ssize_t n;
    fd_set rset, allset;
    char buf[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

    maxfd = listenfd;
    maxi = -1;
    for (i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    for (;;) {
        rset = allset;
        // select 等待某个事件发生：或是新客户连接的建立，或是数据、FIN或RST的到达
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        
        if (FD_ISSET(listenfd, &rset)) {
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
            for (i = 0; i < FD_SETSIZE; i++) {
                if (client[i] < 0) {
                    client[i] = connfd;
                    break;
                }
            }

            if (i == FD_SETSIZE) {
                perr_exit("too many clients");
            }

            FD_SET(connfd, &allset);  // add new descriptor to set
            if (connfd > maxfd) {
                maxfd = connfd;       // for select
            }

            if (i > maxi) {
                maxi = i;             // max index in client[] array
            }

            if (--nready <= 0) {
                continue;             // no more readable descriptors
            }
        }

        for (i = 0; i <= maxi; i++) {
            if ((sockfd = client[i]) < 0) {
                continue;
            }

            if (FD_ISSET(sockfd, &rset)) {
                if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
                    // connection closed by client (read EOF)
                    Close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else {
                    Writen(sockfd, buf, n);
                }

                if (--nready <= 0) {
                    break;            // no more readable descriptors
                }
            }
        }
    }
}