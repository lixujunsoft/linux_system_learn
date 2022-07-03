#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
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
        if (bind(listenfd, res->ai_addr, res->ai_addrlen)) {
            break;
        }
        close(listenfd);
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) {
        printf("tcp_listen error for %s, %s", host, serv);
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