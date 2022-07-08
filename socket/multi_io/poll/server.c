#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include <ctype.h>
#include "wrap.h"

#define MAXLINE 80
#define SERV_PORT 9000
#define OPEN_MAX 1024

/*
int poll(struct pollfd *fdarray, unsigned long nfds, int timeout);

fdarray：指向一个结构数组第一个元素的指针。每个数组元素都是一个pollfd结构，用于指定测试某个给定描述符fd的条件
         struct pollfd {
            int fd;           // 需要检查的文件描述符
            short events;     // 要测试的条件
            short revents;    // 返回该描述符的状态
         };
         每个描述符都有两个变量，一个为调用值，另一个为返回结果，从而避免使用值-结果参数
         poll识别三类数据：普通、优先级带和高优先级
nfds：结构数组中元素的个数（需要监控的元素的个数，注意不是结构数组的大小）
timeout：指定poll函数返回前等待多长时间。它是一个指定应等待毫秒数的正值
         -1 永远等待
         0  立即返回，不阻塞进程
         >0 等待指定数目的毫秒数
         如果系统不能提供毫秒级精度的定时器，该值就向上舍入到最接近的支持值

返回值：
    当发生错误时，poll函数的返回值为-1，若定时器到时之前没有任何描述符就绪，则返回0,否则返回就绪描述符的个数，即revents成员值非0的描述符个数

如果我们不再关心某个特定描述符，那么可以把与它对应的pollfd结构的fd成员设置成一个负值。poll函数将忽略这样的pollfd结构的events成员，
返回时将它的revents成员的值置为0
*/

int main(int argc, char *argv[])
{
    int i, j, maxi, listenfd, connfd, sockfd;
    int nready;
    ssize_t n;
    char buf[MAXLINE], str[INET_ADDRSTRLEN];
    socklen_t clilen;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    Listen(listenfd, 20);

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;   /* listenfd监听普通事件（普通数据可读） */
    maxi = 0;

    for (;;) {
        nready = poll(client, maxi + 1, -1);  // 阻塞
        if (client[0].revents & POLLRDNORM) { // 有客户端链接请求
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
            printf("received from %s at port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));

            for (i = 1; i < OPEN_MAX; i++) {
                if (client[i].fd < 0) {
                    client[i].fd = connfd;    // 找到client[]中空闲的位置，存放accept返回的connfd
                    break;
                }
            }

            if (i == OPEN_MAX) {
                perr_exit("too many clients");
            }

            client[i].events = POLLRDNORM;   // 设置刚刚返回的connfd,监控读事件
            if (i > maxi) {
                maxi = i;
            }

            if (--nready <= 0) {
                continue;
            }
        }

        for (i = 1; i <= maxi; i++) {       // 检测client[]
            if ((sockfd = client[i].fd) < 0) {
                continue;
            }
            // POLLRDNORM 普通数据可读
            // POLLERR    发生错误
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ((n = Read(sockfd, buf, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) { /* 当收到RST标志时 */
                        /* connection reset by client */
                        printf("client[%d]: connection reset by client\n", i);
                        Close(sockfd);
                        client[i].fd = -1;
                    } else {
                        perr_exit("read error");
                    }
                } else if (n == 0) {
                    /* conection closed by client */
                    printf("client[%d]: connection closed by client\n", i);
                    Close(sockfd);
                    client[i].fd = -1;
                } else {
                    for (j = 0; j < n; j++) {
                        buf[j] = toupper(buf[j]);
                    }
                    Write(sockfd, buf, n);
                }

                if (--nready == 0) {
                    break;
                }
            }
        }
    }
}