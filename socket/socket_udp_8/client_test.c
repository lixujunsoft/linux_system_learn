#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include "wrap.h"

int main(int argc, char *argv[])
{
    int sockfd;
    socklen_t len;
    struct sockaddr_in cliaddr, servaddr;

    if (argc != 2) {
        perr_exit("usage: udpcli <IPaddress>");
    }

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    len = sizeof(cliaddr);
    getsockname(sockfd, (struct sockaddr*)&cliaddr, &len);
    printf("local address %s\n", Sock_ntop((struct sockaddr*)&cliaddr, len));
    return 0;
}