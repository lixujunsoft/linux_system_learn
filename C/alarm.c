#include <unistd.h>
#include <stdio.h>

int main()
{
    int n;
    alarm(50);
    sleep(5);
    if ((n = alarm(50)) != 0) {
        printf("last set residue: %d\n", n);
    }
}