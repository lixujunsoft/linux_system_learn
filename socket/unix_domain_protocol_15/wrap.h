#ifndef _WRAP_H_INCLUDED_
#define _WRAP_H_INCLUDED_

#include <sys/socket.h>
#define SERV_PORT 8000
#define MAXLINE 128
#define LISTENQ 128
#define OPEN_MAX 1024
#define INFTIM -1
#define UNIXSTR_PATH "/home/lixujun/linux_system_learn/socket/unix_domain_protocol_15/str_path.txt"
#define UNIXDG_PATH "/home/lixujun/linux_system_learn/socket/unix_domain_protocol_15/dg_path.txt"

typedef void (*Sigfunc)(int);

void perr_exit(const char *s);
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Connect(int fd, const struct sockaddr *sa, socklen_t salen);
void Listen(int fd, int backlog);
int Socket(int family, int type, int protocol);
ssize_t Read(int fd, void *ptr, size_t nbytes);
ssize_t Write(int fd, const void *ptr, size_t nbytes);
void Close(int fd);
ssize_t Readn(int fd, void *vptr, size_t n);
ssize_t Writen(int fd, const void *vptr, size_t n);
Sigfunc Signal(int signo, Sigfunc func);

void sig_chld(int signo);
void str_echo(int connfd);
void str_cli(FILE *fp, int sockfd);
void str_cli_select(FILE *fp, int sockfd);
void str_cli_shutdowm(FILE *fp, int sockfd);
#endif