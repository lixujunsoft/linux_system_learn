#include <errno.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

unsigned long long count;
struct  timeval end;

void err_sys(char *str)
{
    printf("%s\n", str);
    exit(0);
}

void checktime(char *str)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if (tv.tv_sec >= end.tv_sec && tv.tv_usec >= end.tv_usec) {
        printf("%s count = %lld\n", str, count);
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    pid_t pid;
    char *s;
    int nzero, ret;
    int adj = 0;

    nzero = 20;
    setbuf(stdout, NULL);

    printf("NZERO = %d\n", nzero);
    if (argc == 2) {
        adj = strtol(argv[1], NULL, 10);
    }
    gettimeofday(&end, NULL);
    end.tv_sec += 10;  /* run for 10 seconds */
    if ((pid = fork()) < 0) {
        err_sys("fork failed\n");
    } else if (pid == 0) {
        s = "child";
        printf("current nice value in child is %d, adjusting by %d\n", nice(0) + nzero, adj);
        errno = 0;
        ret = nice(adj);
        printf("ret = %d\n", ret);
        printf("ret = %d", errno);
        if (ret == -1 && errno != 0) {
            printf("child set scheduling priority");
        }
        printf("now child nice value is %d\n", ret + nzero);
    } else {
        s = "parent";
        printf("current nice value in parent is %d\n", nice(0) + nzero);
    }

    for (;;) {
        if (++count == 0) {
            err_sys(s);
        }
        checktime(s);
    }
}