#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <error.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>

const char *str = "SOCKEY PAIR TEST.";

int main(int argc, char *argv[])
{
    char buf[128];
    int socket_pair[2];
    pid_t pid;

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, socket_pair)) {
        printf("Error, socketpair create failed, errno(%d):%s\n", errno, strerror(errno));
        return -1;
    }
    // 写
    int size = write(socket_pair[0], str, strlen(str));
    // 读
    read(socket_pair[1], buf, size);
    printf("Read result: %s\n", buf);

    return 0;
}