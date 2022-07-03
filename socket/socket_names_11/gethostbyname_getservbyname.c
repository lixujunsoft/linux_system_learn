#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "wrap.h"

#define MAXLINE 128

int main(int argc, char *argv[])
{
    int sockfd, n;
    char recvline[MAXLINE];
    struct sockaddr_in servaddr;
    struct in_addr **pptr;
    struct in_addr *inetaddrp[2];
    struct in_addr inetaddr;
    struct hostent *hp;
    struct servent *sp;

    if (argc != 3) {
        printf("usage: daytimetcpcli1 <hostname> <service>\n");
        return 0;
    }

    if ((hp = gethostbyname(argv[1])) == NULL) {
        if (inet_aton(argv[1], &inetaddr) == 0) {
            printf("hostname error for %s: %s", argv[1], hstrerror(h_errno));
        } else {
            inetaddrp[0] = &inetaddr;
            inetaddrp[1] = NULL;
            pptr = inetaddrp;
        }
    } else {
        pptr = (struct in_addr**)hp->h_addr_list;
    }

    if ((sp = getservbyname(argv[2], "tcp")) == NULL) {
        printf("getservbyname error for %s\n", argv[2]);
    }
    printf("server port: %d\n", ntohs(sp->s_port));

    for (; *pptr != NULL; pptr++) {
        sockfd = Socket(AF_INET, SOCK_STREAM, 0);
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = sp->s_port;
        memcpy(&servaddr.sin_addr, *pptr, sizeof(struct in_addr));
        printf("trying %s\n", Sock_ntop((struct sockaddr*)&servaddr, sizeof(servaddr)));
        if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == 0) {
            break;   /* success */
        }

        printf("connect error\n");
        close(sockfd);
    }

    if (*pptr == NULL) {
        printf("unable to connect\n");
    }

    while ((n = Read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
        fputs(recvline, stdout);
    }

    return 0;
}