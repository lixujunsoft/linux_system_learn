#include <sys/resource.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int nice = getpriority(PRIO_USER, getpid());
    printf("nice = %d\n", nice);
}