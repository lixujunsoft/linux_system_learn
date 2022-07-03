#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "getaddrinfo.h"
#include "wrap.h"

int main(int argc, char *argv[])
{
    int sockfd;
    ssize_t n;
    char buff[MAXLINE];
    time_t ticks;
    socklen_t len;
    struct sockaddr_storage cliaddr;

    if (argc == 2) {
        sockfd = udp_server(NULL, argv[1], NULL);
    } else if (argc == 3) {
        sockfd = udp_server(argv[1], argv[2], NULL);
    } else {
        printf("usage: daytime [ <host> ] <service or port>\n");
    }

    for (;;) {
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, buff, MAXLINE, 0, (struct sockaddr*)&cliaddr, &len);
        printf("datagram form %s:%d\n", Sock_ntop((struct sockaddr*)&cliaddr, len), Sock_port((struct sockaddr*)&cliaddr, len));
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&cliaddr, len);
    }
}