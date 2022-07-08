#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "wrap.h"

#define MAXLINE 80
#define SERV_PORT 10000

/*
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
nfds:监控的文件描述符集里最大文件描述符加1,因为此参数会告诉内核检测前多少个文件描述符的状态
readfds:监控有读数据到达文件描述符集合，传入传出参数
writefds:监控写数据到达文件描述符集合，传入传出参数
exceptfds:监控异常发生到达文件描述符集合，如带外数据到达异常，传入传出参数
timeout:定时阻塞监控时间，3种情况
    1.NULL，永远等下去
    2.设置timeval，等待固定时间
    3.设置timeval里时间均为0,检查描述符后立即返回，轮询

struct timeval {
    long tv_sec;       // seconds
    long tv_usec;      // microseconds
}  

select的中间3个参数（指向描述符集的指针）中的任意一个（或全部）可以是空指针，这表示对相应条件并不关心。如果所有三个指针都是NULL，
则select提供了比sleep更精确的定时器

返回值：
1.返回值-1表示出错。在所指定的描述符一个都没有准备好时，捕捉到一个信号。此种情况下，一个描述符集都不会改
2.返回0表示没有描述符准备好。若指定的描述符一个都没准备好，指定的时间就过了，那么就会发生这种情况。此时，所有描述符集都会清零
3.一个正返回值说明了已经准备好的描述符数。该值是3个描述符集中已准备好的描述符之和，所以如果同一描述符已准备好读和写，那么在返回值中
  会对其计两次数。在这种情况下，3个描述符集中仍旧打开的位对应于已准备好的描述符。

准备好：
1.若对读集(readfds)中的一个描述符进行的read操作不会阻塞，则认为此描述符是准备好的
2.若对写集(writefds)中的一个描述符进行的write操作不会阻塞，则认为此描述符是准备好的
3.若对异常条件集(exceptfds)中的一个描述符有一个未决异常条件，则认为此描述符是准备好的。异常条件包括：某个套接字的带外数据到达，
  或者某个已置为分组模式的伪终端存在可从其主端读取的控制状态信息
4.对于读、写和异常条件，普通文件的描述符总是返回准备好
*/

int main(int argc, char *argv[])
{
    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];   // FD_SETSIZE 默认为1024
    ssize_t n;
    fd_set rset, allset;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];        // #define INET_ADDRSTRLEN 16
    socklen_t cliaddr_len;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (struct sockaddr*)&servaddr,  sizeof(servaddr));

    Listen(listenfd, 20);   // 默认最大128

    maxfd = listenfd;
    maxi = -1;
    
    for (i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);    // 构造select监控文件描述符集

    for (;;) {
        rset = allset;
        
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        if (nready < 0) {
            perr_exit("select error");
        }

        if (FD_ISSET(listenfd, &rset)) { // new client connection
            cliaddr_len = sizeof(cliaddr);
            connfd = Accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddr_len);
            printf("received from %s at port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));

            for (i = 0; i < FD_SETSIZE; i++) {
                if (client[i] < 0) {
                    client[i] = connfd;  // 保存accpet返回的文件描述符到client[]里
                    break;
                }
            }
            
            if (i == FD_SETSIZE) {
                    printf("too many clients\n");
                    exit(1);
            }

            FD_SET(connfd, &allset);
            if (connfd > maxfd) {
                maxfd = connfd;
            }

            if (i > maxi) {
                maxi = i;
            }

            if (--nready == 0) {      // 就绪文件描述符只有一个，为listenfd
                continue;
            }
        }

        for (i = 0; i <= maxi; i++) { // 检测哪个clients有数据就绪
            if ((sockfd = client[i]) < 0) {
                continue;
            }

            if (FD_ISSET(sockfd, &rset)) {
                /*
                1.如果对端TCP发送数据，那么该套接字变为可读，并且read返回一个大于0的值（即读入数据的字节数）
                2.如果对端TCP发送一个FIN（对端进程终止），那么该套接字变为可读，并且read返回0（EOF）
                3.如果对端TCP发送一个RST（对端主机崩溃并重新启动），那么该套接字变为可读，并且read返回-1,而errno中含有确切的错误码
                */
                if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
                    // 当client关闭链接时，服务器端也关闭对应链接
                    Close(sockfd);
                    FD_CLR(sockfd, &allset);   // 解除select监控此文件描述符
                    client[i] = -1;
                } else {
                    int j;
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

    Close(listenfd);
    return 0;
}