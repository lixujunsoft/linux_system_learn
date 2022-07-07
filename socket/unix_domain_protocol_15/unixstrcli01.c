#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <error.h>
#include "wrap.h"

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_un servaddr;

    sockfd = Socket(AF_LOCAL, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);

    Connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    str_cli(stdin, sockfd);
    exit(0);
}