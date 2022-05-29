#include <stdio.h>
#include <sys/types.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void pr_exit(int status)
{
    if (WIFEXITED(status)) { // 若为正常终止子进程返回的状态，则为真
        /* WEXITSTATUS(status) 获取子进程传送给exit或_exit参数的低8位 */
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) { // 若为异常终止子进程返回的状态，则为真（接到一个不捕捉的信号）
        /* 对于这种情况，可执行WTERMSIG(status)，获取使子进程终止的信号编号 */
        printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status), 
#ifdef WCOREDUMP
            /* WCOREDUMP(status)  若已产生终止进程的core文件，则它返回真 */
            WCOREDUMP(status) ? "(core file generated)" : "");
#else        
        "");
#endif
    } else if (WIFSTOPPED(status)) { // 若为当前暂停子进程的返回的状态，则为真。对于这种
        //  情况，可执行WSTOPSIG(status)，获取使子进程暂停的信号编号
        printf("child stopped, signal number = %d\n", WSTOPSIG(status));
    }
}

void test()
{
    pid_t pid;
    int status;
    if ((pid = fork()) < 0) {
        printf("fork error\n");
    } else if (pid == 0) {
        status /= 0;        // divide by 0 generates SIGFPE
    }

    if (wait(&status) != pid) {
        printf("wait error\n");
    }
    pr_exit(status);
}

int main()
{
    pid_t pid;
    int status;

/*
    if ((pid = fork()) < 0) {
        printf("fork error\n");
    } else if (pid == 0) {
        exit(7);
    }

    if (wait(&status) != pid) {
        printf("wait error\n");
    }
    pr_exit(status);

    if ((pid = fork()) < 0) {
        printf("fork error\n");
    } else if (pid == 0) {
        abort();             // generates SIGABRT
    }

    if (wait(&status) != pid) {
        printf("wait error\n");
    }
    pr_exit(status);
*/
    test();
    
    return 0;
}