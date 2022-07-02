#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include "wrap.h"

static void recvfrom_int(int signo);
static int count;

void dg_echo(int sockfd, struct sockaddr* pcliaddr, socklen_t clilen)
{
    int n;
    socklen_t len;
    char mesg[MAXLINE];

    for (;;) {
        len = clilen;
        n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
        printf("receive form %s\n", Sock_ntop(pcliaddr, clilen));
        sendto(sockfd, mesg, n, 0, pcliaddr, len);
    }
}

void dg_echo_test(int sockfd, struct sockaddr* pcliaddr, socklen_t clilen)
{
    socklen_t len;
    char mesg[MAXLINE];

    Signal(SIGINT, recvfrom_int);

    for (;;) {
        len = clilen;
        recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
        count++;
    }
}

void dg_large_buf(int sockfd, struct sockaddr* pcliaddr, socklen_t clilen)
{
    int n;
    socklen_t len;
    char mesg[MAXLINE];

    Signal(SIGINT, recvfrom_int);
    n = 220 * 1024;

    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &n, sizeof(n));
    for (;;) {
        len = clilen;
        recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
        count++;
    }
}

static void recvfrom_int(int signo)
{
    printf("\nreceived %d datagrams\n", count);
}

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    
    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    dg_large_buf(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
}