#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include "wrap.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void perr_exit(const char *s)
{
    perror(s);
    exit(1);
}

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    int n;
again:
    if ((n = accept(fd, sa, salenptr)) <0) {
        if ((errno == ECONNABORTED) || (errno == EINTR)) {
            goto again;
        } else {
            perr_exit("accept error");
        }
    }

    return n;
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if (bind(fd, sa, salen) < 0) {
        perr_exit("bind error");
    }
}

void Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if (connect(fd, sa, salen) < 0) {
        perr_exit("connect error");
    }
}

void Listen(int fd, int backlog)
{
    if (listen(fd, backlog) < 0) {
        perr_exit("listen error");
    }
}

int Socket(int family, int type, int protocol)
{
    int n;
    if ((n = socket(family, type, protocol)) < 0) {
        perr_exit("socket error");
    }
    return n;
}

ssize_t Read(int fd, void *ptr, size_t nbytes)
{
    ssize_t n;
again:
    if ((n = read(fd, ptr, nbytes)) == -1) {
        if (errno == EINTR) {
            goto again;
        } else {
            return -1;
        }
    }
}
ssize_t Write(int fd, const void *ptr, size_t nbytes)
{
    ssize_t n;
again:
    if ((n = write(fd, ptr, nbytes)) == -1) {
        if (errno == EINTR) {
            goto again;
        } else {
            return -1;
        }
    }
    return n;
}

void Close(int fd)
{
    if (close(fd) == -1) {
        perr_exit("close error");
    }
}

ssize_t Readn(int fd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR) {
                nread = 0;
            } else {
                return -1;
            }
        } else if (nread == 0) {
            break;
        } 
        nleft -= nread;
        ptr += nread;
    }

    return n - nleft;   
}

ssize_t Writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR) {
                nwritten = 0;
            } else {
                return -1;
            }
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

Sigfunc Signal(int signo, Sigfunc func)
{
    struct sigaction act, oact;
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;
#endif
    } else {
#ifdef SA_RESTART
    act.sa_flags |= SA_RESTART;
#endif
    }

    if (sigaction(signo, &act, &oact) < 0) {
        return(SIG_ERR);
    }
    return oact.sa_handler;
}

/*
waitpid(pid_t pid, int *statloc, int options);
pid == -1 等待任一子进程
WNOHANG   若由pid指定的子进程不是立即可用的，则waitpid不阻塞，此时其返回值为0
*/
void sig_chld(int signo)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        printf("child %d terminated\n", pid);
    }
    return;
}

void str_echo(int connfd)
{
    ssize_t n;
    char buf[MAXLINE];

again:
    while ((n = read(connfd, buf, MAXLINE)) > 0) {
        Writen(connfd, buf, n);
    }

    if (n < 0 && errno == EINTR) {
        goto again;
    } else if (n < 0) {
        perr_exit("str_eoch: read error");
    }
}

void str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        Writen(sockfd, sendline, strlen(sendline));
        if (Read(sockfd, recvline, MAXLINE) == 0) {
            perr_exit("str_cli: server terminated prematurely");
        }
        fputs(recvline, stdout);
    }
}

void str_cli_select(FILE *fp, int sockfd)
{
    int maxfdp1;
    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];

    FD_ZERO(&rset);
    for (;;) {
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = (fileno(fp) > sockfd ? fileno(fp) : sockfd) + 1;
        select(maxfdp1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)) {
            bzero(recvline, sizeof(sendline));   // 注：读数据之前清空
            if (Read(sockfd, recvline, MAXLINE) == 0) {
                perr_exit("str_cli: server terminated prematurely");
            }
            fputs(recvline, stdout);
        }

        if (FD_ISSET(fileno(fp), &rset)) {
            bzero(sendline, sizeof(sendline));
            if (fgets(sendline, MAXLINE, fp) == NULL) {
                return;
            }
            Writen(sockfd, sendline, strlen(sendline));
        }
    }
}

void str_cli_shutdowm(FILE *fp, int sockfd)
{
    int maxfdp1, stdineof;
    fd_set rset;
    char buf[MAXLINE];
    int n;

    stdineof = 0;
    FD_ZERO(&rset);
    for (;;) {
        if (stdineof == 0) {
            FD_SET(fileno(fp), &rset);
        }
        FD_SET(sockfd, &rset);
        maxfdp1 = (fileno(fp) > sockfd ? fileno(fp) : sockfd) + 1;
        select(maxfdp1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(sockfd, &rset)) {
            if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
                if (stdineof == 1) {
                    return;
                } else {
                    perr_exit("str_cli: server terminated prematurely");
                }
            }
            Write(fileno(stdout), buf, n);
        }

        if (FD_ISSET(fileno(fp), &rset)) {
            if ((n = Read(fileno(fp), buf, MAXLINE)) == 0) {
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            Writen(sockfd, buf, n);
        }
    }
}

const char *Sock_ntop(struct sockaddr* addr, socklen_t len)
{
    char buf[INET6_ADDRSTRLEN];
    if (sizeof(struct sockaddr_in) == len) {
        struct sockaddr_in *addrTmp = (struct sockaddr_in*)addr; 
        return inet_ntop(AF_INET, &addrTmp->sin_addr, buf, sizeof(buf));
    } else if (sizeof(struct sockaddr_in6) == len) {
        struct sockaddr_in6 *addrTmp = (struct sockaddr_in6*)addr; 
        return inet_ntop(AF_INET6, &addrTmp->sin6_addr, buf, sizeof(buf));
    }

    return NULL;
}

int Sock_port(struct sockaddr* addr, socklen_t len)
{
    if (sizeof(struct sockaddr_in) == len) {
        struct sockaddr_in *addrTmp = (struct sockaddr_in*)addr; 
        return ntohs(addrTmp->sin_port);
    } else if (sizeof(struct sockaddr_in6) == len) {
        struct sockaddr_in6 *addrTmp = (struct sockaddr_in6*)addr;
        return ntohs(addrTmp->sin6_port);
    }

    return -1;
}