#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "wrap.h"

struct addrinfo *host_serv(const char *host, const char *serv, int family, int socktype)
{
    int n;
    struct addrinfo hints, *res;
    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_CANONNAME;   // 告知getaddrinfo函数返回主机的规范名字
    hints.ai_family = family;        // AF_UNSPEC, AF_INET, AF_INET6
    hints.ai_socktype = socktype;    // SOCK_STREAM, SOCK_DGRAM

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        return NULL;
    }
    return res;
}

int tcp_connect(const char *host, const char *serv)
{
    int sockfd, n;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        printf("tcp_connect error for %s, %s: %s", host, serv, gai_strerror(n));
        return 0;
    }
    ressave = res;

    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0) {
            continue;
        }

        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) {
            break;
        }

        close(sockfd);
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) {
        printf("tcp_connect error for %s, %s", host, serv);
        return 0;
    }

    freeaddrinfo(ressave);
    return sockfd;
}

int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp)
{
    int listenfd, n;
    const int on = 1;
    struct addrinfo hints, *res, *ressave;
    char buf[MAXLINE];

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        printf("tcp_listen error for %s, %s: %s", host, serv, gai_strerror(n));
        return -1;
    }

    ressave = res;

    do {
        listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (listenfd < 0) {
            continue;
        }

        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        printf("serveraddr %s\n", Sock_ntop(res->ai_addr, res->ai_addrlen));
        printf("port: %d\n", Sock_port(res->ai_addr, res->ai_addrlen));
        if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0) {
            break;
        }
        perror("Bind");
        close(listenfd);
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) {
        printf("tcp_listen error for %s, %s\n", host, serv);
        return -1;
    }

    listen(listenfd, LISTENQ);

    if (addrlenp) {  // 如果addrlenp参数非空，通过这个参数返回协议地址的大小。这个大小允许调用者通过accept获取
                     // 客户的协议地址时分配一个套接字地址结构的内存空间
        *addrlenp = res->ai_addrlen;
    }

    freeaddrinfo(ressave);
    return listenfd;
}

int udp_client(const char *host, const char *serv, struct sockaddr** saptr, socklen_t *lenp)
{
    int sockfd, n;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        printf("udp_client error for %s, %s: %s\n", host, serv, gai_strerror(n));
    }

    ressave = res;

    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd >= 0) {
            break;
        }
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) {
        printf("udp_client error for %s, %s", host, serv);
    }

    *saptr = malloc(res->ai_addrlen);
    memcpy(*saptr, res->ai_addr, res->ai_addrlen);
    *lenp = res->ai_addrlen;

    freeaddrinfo(ressave);
    return sockfd;
}

int udp_connect(const char *host, const char *serv)
{
    int sockfd, n;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        printf("udp_connect error for %s, %s: %s", host, serv, gai_strerror(n));
        return -1;
    }

    ressave = res;

    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0) {
            continue;
        }
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) {
            break;
        }
        close(sockfd);
    } while ((res = res->ai_next) != NULL);
    
    if (res == NULL) {
        printf("udp_connect error for %s, %s", host, serv);
    }

    freeaddrinfo(ressave);
    return sockfd;
}

int udp_server(const char *host, char *serv, socklen_t *addrlenp)
{
    int sockfd, n;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;   // 套接字将用于被动打开
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        printf("udp_server error for %s, %s: %s", host, serv, gai_strerror(n));
        return -1;
    }

    ressave = res;

    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0) {
            continue;
        }

        printf("serveraddr %s\n", Sock_ntop(res->ai_addr, res->ai_addrlen));
        printf("port: %d\n", Sock_port(res->ai_addr, res->ai_addrlen));
        if (bind(sockfd, res->ai_addr, res->ai_addrlen) == 0) {
            break;
        }

        close(sockfd);
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) {
        printf("udp_server error for %s, %s", host, serv);
    }

    if (addrlenp) {
        *addrlenp = res->ai_addrlen;
    }

    freeaddrinfo(ressave);
    return sockfd;
}