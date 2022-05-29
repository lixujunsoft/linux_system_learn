#include <stdio.h>
#include <sys/types.h>
#include <sys/types.h>
#include <unistd.h>

int globvar = 6;
char buf[] = "a write to stdout\n";

int main(void)
{
    int var;
    pid_t pid;
    var = 88;
    if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1) {
        printf("write error");
    }

    printf("before fork\n");

    if ((pid = fork()) < 0) {
        printf("fork error");
    } else if (pid == 0)
    {
        globvar++;
        var++;
    }

    write(STDOUT_FILENO, buf, sizeof(buf) - 1);
    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
    return 0;
}
