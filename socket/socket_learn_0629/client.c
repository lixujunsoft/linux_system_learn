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

void str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        Writen(sockfd, sendline, strlen(sendline));
        if (Read(sockfd, recvline, MAXLINE) == 0) {
            perr_exit("str_cli: server terminated prematurely");
        }
        fputs(recvline, stdout);
    }
}

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;
    if (argc != 2) {
        perr_exit("usage: tcpcli <IPaddress>");
    }

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    Connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    str_cli(stdin, sockfd);
    exit(0);
}