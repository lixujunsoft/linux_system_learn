#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "getaddrinfo.h"
#include "wrap.h"

int main(int argc, char *argv[])
{
    int listenfd, connfd;
    socklen_t len, addrlen;
    char buff[MAXLINE];
    time_t ticks;
    struct sockaddr_storage cliaddr;

    if (argc == 2) {
        listenfd = tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 3) {
        listenfd = tcp_listen(argv[1], argv[2], &addrlen);
    } else {
        printf("usage: daytimetcpsrv2 [ <host> ] <service or port>\n");
        return -1;
    }

    for (;;) {
        len = sizeof(cliaddr);
        connfd = Accept(listenfd, (struct sockaddr*)&cliaddr, &len);
        printf("connection form %s\n", Sock_ntop((struct sockaddr*)&cliaddr, len));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));
        Close(connfd);
    }
}