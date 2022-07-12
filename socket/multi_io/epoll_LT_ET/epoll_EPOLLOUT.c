#include <unistd.h>
#include <stdio.h>
#include <sys/epoll.h>

void test4()
{
    int epfd, nfds;
    struct epoll_event ev, events[5];
    epfd = epoll_create(1);
    ev.data.fd = STDOUT_FILENO;
    ev.events = EPOLLOUT | EPOLLET;     // 监听读状态同时设置ET模式
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDOUT_FILENO, &ev);
    for (;;) {
        nfds = epoll_wait(epfd, events, 5, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd = STDOUT_FILENO) {
                printf("hello world");
            }

            ev.data.fd = STDOUT_FILENO;
            ev.events = EPOLLOUT | EPOLLET;
            epoll_ctl(epfd, EPOLL_CTL_MOD, STDOUT_FILENO, &ev);  // 重新MOD事件(ADD无效)
        }
    }
}

/*
LT模式下只要输出缓存区有写空间就返回写就绪；当输出缓存区满的时候，会自动刷新输出，同样会造成epoll_wait返回写就绪
*/
void test3()
{
    int epfd, nfds;
    struct epoll_event ev, events[5];
    epfd = epoll_create(1);
    ev.data.fd = STDOUT_FILENO;
    ev.events = EPOLLOUT;              // 默认使用LT模式
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDOUT_FILENO, &ev);
    for (;;) {
        nfds = epoll_wait(epfd, events, 5, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd = STDOUT_FILENO) {
                printf("hello world");
            }
        }
    }
}

void test2()
{
    int epfd, nfds;
    struct epoll_event ev, events[5];
    epfd = epoll_create(1);
    ev.data.fd = STDOUT_FILENO;
    ev.events = EPOLLOUT | EPOLLET;     // 监听读状态同时设置ET模式
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDOUT_FILENO, &ev);
    for (;;) {
        nfds = epoll_wait(epfd, events, 5, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd = STDOUT_FILENO) {
                printf("hello world");  // 相对于test1()中的代码，去掉了换行操作
            }
        }
    }
}

void test1()
{
    int epfd, nfds;
    struct epoll_event ev, events[5];
    epfd = epoll_create(1);
    ev.data.fd = STDOUT_FILENO;
    ev.events = EPOLLOUT | EPOLLET;     // 监听读状态同时设置ET模式
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDOUT_FILENO, &ev);
    for (;;) {
        nfds = epoll_wait(epfd, events, 5, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd = STDOUT_FILENO) {
                printf("hello world\n");
            }
        }
    }
}

int main(void)
{
    // test1();
    // test2();
    test3();
}

