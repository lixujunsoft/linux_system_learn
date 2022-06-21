#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define MAXLINE 80
#define SERV_PORT 10000

int listenfd;

void handler(int num)
{
    switch (num) {
    case SIGINT:
        close(listenfd);
        printf("Receive SIGINT: process done\n");
        printf("请两分钟后重启服务器\n");
        exit(0);
        break;
    default:
        break;
    }
}

int main()
{
    signal(SIGINT, handler);
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];
    int i, n;
    int connfd;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 20);

    printf("Accepting connections ...\n");
    while (1) {
        cliaddr_len = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddr_len);
        printf("Received from %s at Port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));
        close(connfd);
    }
    return 0;
}