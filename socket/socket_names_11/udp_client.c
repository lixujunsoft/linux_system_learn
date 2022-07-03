#include <stdio.h>
#include "getaddrinfo.h"
#include "wrap.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int sockfd, n;
    char recvline[MAXLINE + 1];
    socklen_t salen;
    struct sockaddr *sa;

    if (argc != 3) {
        printf("usage: daytime <hostname/IPaddress> <service/port>");
        return -1;
    }

    sockfd = udp_client(argv[1], argv[2], &sa, &salen);
    printf("sending to %s:%d\n", Sock_ntop(sa, salen), Sock_port(sa, salen));
    sendto(sockfd, "", 1, 0, sa, salen);
    n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
    recvline[n] = '\0';
    fputs(recvline, stdout);
    exit(0);
}