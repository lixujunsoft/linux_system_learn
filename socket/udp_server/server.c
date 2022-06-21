#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXLINE 80
#define SERV_PORT 8000

int sockfd;

void handler(int num)
{
    switch (num) {
    case SIGINT:
        close(sockfd);
        printf("Receive SIGINT: process done\n");
        printf("请两分钟后重启服务器\n");
        exit(0);
        break;
    default:
        break;
    }
}

int main(void)
{
    signal(SIGINT, handler);
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];
    int i, n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    printf("Accepting connections ...\n");
    while (1) {
        cliaddr_len = sizeof(cliaddr);
        n = recvfrom(sockfd, buf, MAXLINE, 0, (struct sockaddr*)&cliaddr, &cliaddr_len);
        if (n == -1) {
            printf("recvfrom error\n");
        }
        printf("received from %s at port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));
    }

    close(sockfd);
    return 0;
}