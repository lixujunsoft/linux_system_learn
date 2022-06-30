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

void str_cli_test(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        Writen(sockfd, sendline, strlen(sendline));
        sleep(1);
        Writen(sockfd, sendline + 1, strlen(sendline) - 1);
        if (Read(sockfd, recvline, MAXLINE) == 0) {
            perr_exit("str_cli_test: server terminated prematurely");
        }
        fputs(recvline, stdout);
    }
}

void sig_pipe(int signo)
{
    printf("capture SIGPIPE\n");
}

int main(int argc, char *argv[])
{
    signal(SIGPIPE, sig_pipe);
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
    str_cli_test(stdin, sockfd);
    exit(0);
}