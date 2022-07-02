#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include "wrap.h"

/* dg_cli函数是协议无关的 
   dg_cli不查看这个协议相关结构的内容，而是简单地把一个指向该结构的指针传递给sendto

   内核给套接字指派一个临时端口
   1.TCP套接字：connect调用时，如果没有绑定一个本地端口，内核就在此时为它选择一个临时端口
   2.UDP套接字：进程首次调用sendto时，如果没有绑定一个本地端口，内核就在此时为它选择一个临时端口
   注：TCP、UDP客户也可以显式地调用bind，但很少这么做
*/
void dg_cli(FILE *fp, int sockfd, struct sockaddr *pservaddr, socklen_t servlen)
{
    int n; 
    char sendline[MAXLINE], recvline[MAXLINE];
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        bzero(recvline, sizeof(recvline));
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
        recvline[n] = 0;

        bzero(sendline, sizeof(sendline));
        fputs(recvline, stdout);
    }
}

void dg_cli_check(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    int n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    socklen_t len;
    struct sockaddr *preply_addr;
    preply_addr = malloc(servlen);

     while (fgets(sendline, MAXLINE, fp) != NULL) {
        bzero(recvline, sizeof(recvline));
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        
        len = servlen;
        n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
        if (len != servlen || memcmp(pservaddr, preply_addr, len) != 0) {
            printf("ignored %s\n", Sock_ntop(preply_addr, len));
            //continue;
        }

        recvline[n] = 0;

        bzero(sendline, sizeof(sendline));
        fputs(recvline, stdout);
    }
}

// 第8章，基本UDP套接字编程(p200页)
void dg_cli_revise(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    printf("dg_cli_revise\n");
    int n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    connect(sockfd, (struct sockaddr*)pservaddr, servlen);

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        bzero(recvline, sizeof(recvline));
        Write(sockfd, sendline, strlen(sendline));
        if ((n = Read(sockfd, recvline, MAXLINE)) < 0) {
            if (errno == ECONNREFUSED) {
                perr_exit("read error");
            }
        }
        recvline[n] = 0;
        fputs(recvline, stdout);
        bzero(sendline, sizeof(sendline));
    }
}

void dg_cli_test(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    int i;
    char sendline[DGLEN];
    for (i = 0; i < NDG; i++) {
        sendto(sockfd, sendline, DGLEN, 0, pservaddr, servlen);
    }
}

/* main函数是协议相关的 */
int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;
    if (argc != 2) {
        perr_exit("usage: udpcli <IPaddress>");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    //dg_cli(stdin, sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    dg_cli_test(stdin, sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    exit(0);
}