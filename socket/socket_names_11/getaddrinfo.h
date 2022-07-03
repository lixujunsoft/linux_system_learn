#ifndef _GETADDRINFO_H_INCLUDED_
#define _GETADDRINFO_H_INCLUDED_

#include <sys/socket.h>
#include <netinet/in.h>

struct addrinfo *host_serv(const char *host, const char *serv, int family, int socktype);
int tcp_connect(const char *host, const char *serv);
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);
int udp_client(const char *host, const char *serv, struct sockaddr** saptr, socklen_t *lenp);
int udp_server(const char *host, char *serv, socklen_t *addrlenp);

#endif