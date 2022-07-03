#ifndef _GETADDRINFO_H_INCLUDED_
#define _GETADDRINFO_H_INCLUDED_

#include <sys/socket.h>

struct addrinfo *host_serv(const char *host, const char *serv, int family, int socktype);
int tcp_connect(const char *host, const char *serv);
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

#endif