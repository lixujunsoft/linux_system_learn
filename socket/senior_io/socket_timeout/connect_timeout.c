#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <sys/select.h>
#include "wrap.h"

static void sig_alarm(int);

/* 使用本技术总能减少connect的超时期限，但是无法延长内核现有的超时 
   connect_timeout_alarm(...) 在未线程化或单线程化的程序中使用
*/
int connect_timeout_alarm(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec)
{
    Sigfunc *sigfunc;
    int n;

    sigfunc = Signal(SIGALRM, sig_alarm);
    if (alarm(nsec) != 0) { // 若果此前已经给本进程设置过报警时钟，那么alarm的返回值是这个报警时钟当前的秒数。如果之前没有设置，则alarm返回0
        printf("connect_timeout: alarm was already set\n");
    }

    if ((n = connect(sockfd, saptr, salen)) < 0) {
        close(sockfd);
        if (errno == EINTR) {   // connect系统调用被信号打断
            errno = ETIMEDOUT;
        }
    }

    alarm(0);   /* turn off the alarm */
    return n;
}

static void sig_alarm(int signum)
{
    return;    /* just interrupt the connect() */
}

void dg_cli_alarm(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    int n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    Signal(SIGALRM, sig_alarm);
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        alarm(5);
        if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) < 0) {
            if (errno == EINTR) {
                fprintf(stderr, "socket timeout\n");
            } else {
                printf("recvfrom error\n");
            }
        } else {
            alarm(0);
            recvline[n] = 0;
            fputs(recvline, stdout);
        }
    }
}

/* 使用select为recvfrom设置超时 */
int readable_timeout(int fd, int sec)
{
    fd_set rset;
    struct timeval tv;
    FD_ZERO(&rset);
    FD_SET(fd, &rset);

    tv.tv_sec = sec;
    tv.tv_usec = 0;
    return select(fd + 1, &rset, NULL, NULL, &tv); /* > 0 if descriptor is readable */
}

void dg_cli_select(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    int n;
    char sendline[MAXLINE], recvline[MAXLINE];

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        if (readable_timeout(sockfd, 5) == 0) {
            fprintf(stderr, "socket timeout\n");
        } else {
            n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
            recvline[n] = 0;
            fputs(recvline, stdout);
        }
    }
}

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
    int n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    struct timeval tv;

    tv.tv_sec = 5;
    tv.tv_usec = 0;

    // SO_RCVTIMEO 接收超时 SO_SNDTIMEO 发送超时
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
        if (n < 0) {
            if (errno == EWOULDBLOCK) {
                fprintf(stderr, "socket timeout\n");
                continue;
            } else {
                printf("recvfrom error\n");
            }
        }

        recvline[n] = 0;
        fputs(recvline, stdout);
    }
}