#include <unistd.h>
#include <stdio.h>
#include <sys/epoll.h>

void test4()
{
     int epfd, nfds;
    char buf[256];
    struct epoll_event ev, events[5];
    epfd = epoll_create(1);
    ev.data.fd = STDIN_FILENO;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

    for (;;) {
        nfds = epoll_wait(epfd, events, 5, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == STDIN_FILENO) {
                printf("%s\n", "hello");
                /*ev.data.fd = STDIN_FILENO;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_MOD, STDIN_FILENO, &ev);   // 重新MOD事件（ADD无效）*/
            }
        }
    }
}

void test3()
{
    int epfd, nfds;
    char buf[256];
    struct epoll_event ev, events[5];   // ev用于注册事件，数组用于返回要处理的事件
    epfd = epoll_create(1);             // 只需要监听一个描述符——标准输入
    ev.data.fd = STDIN_FILENO;
    ev.events = EPOLLIN;                   // 默认LT模式
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

    for (;;) {
        nfds = epoll_wait(epfd, events, 5, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == STDIN_FILENO) {
                read(STDIN_FILENO, buf, sizeof(buf));
                printf("%s\n", "hello");
            }
        }
    }
}

void test2()
{
    int epfd, nfds;
    struct epoll_event ev, events[5];   // ev用于注册事件，数组用于返回要处理的事件
    epfd = epoll_create(1);             // 只需要监听一个描述符——标准输入
    ev.data.fd = STDIN_FILENO;
    ev.events = EPOLLIN;                // 使用默认LT模式
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

    for (;;) {
        nfds = epoll_wait(epfd, events, 5, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == STDIN_FILENO) {
                printf("%s\n", "hello");
            }
        }
    }
}

void test1()
{
    int epfd, nfds;
    struct epoll_event ev, events[5];   // ev用于注册事件，数组用于返回要处理的事件
    epfd = epoll_create(1);             // 只需要监听一个描述符——标准输入
    ev.data.fd = STDIN_FILENO;
    ev.events = EPOLLIN | EPOLLET;      // 监听读状态同时设置ET模式
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

    for (;;) {
        nfds = epoll_wait(epfd, events, 5, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == STDIN_FILENO) {
                printf("%s\n", "hello");
            }
        }
    }
}

int main(void)
{
    // test1();
    // test2();
    // test3();
    test4();
}