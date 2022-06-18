#include "ngx_typedef.h"
#include "ngx_core.h"
#include <signal.h>
#include <time.h>
#include "ngx_typedef.h"
#include "ngx_core.h"

ngx_pid_t     ngx_pid;
ngx_pid_t     ngx_parent;

sig_atomic_t  ngx_terminate;
sig_atomic_t  ngx_quit;
ngx_uint_t    ngx_exiting;
sig_atomic_t  ngx_reopen;

ngx_pid_t ngx_spawn_process(ngx_spawn_proc_pt proc, void *data, char *name);
static void ngx_worker_process_cycle(void *data);
static void ngx_start_worker_processes(ngx_int_t n);
static void ngx_worker_process_init(ngx_int_t worker);
static void ngx_worker_process_exit();

void ngx_master_process_cycle(ngx_int_t n)
{
    sigset_t           set;

    /* 设置能接收到的信号 */
    /* 初始化由set指向的信号集，清除其中所有信号 */
    sigemptyset(&set);

    /* sigaddset将一个信号添加到set指向的信号集中 */
    sigaddset(&set, SIGCHLD);
    sigaddset(&set, SIGALRM);
    sigaddset(&set, SIGIO);
    sigaddset(&set, SIGINT);

    /* sigprocmask可以检测或更改进程的信号屏蔽字 
       若set是一个非空指针，则参数how指示如何修改当前的信号屏蔽字
       SIG_BLOCK 该进程新的信号屏蔽字是当前信号屏蔽字和set指向型号集合的并集。set包含了希望阻塞的附加信号

       更改后子进程将会继承父进程的信号屏蔽字
    */
    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
        printf("sigprocmask() failed");
    }

    sigemptyset(&set);

    ngx_start_worker_processes(n);

    /* 主线程循环 */
    for ( ;; ) {
        sleep(1);
    }
}

static void ngx_start_worker_processes(ngx_int_t n)
{
    ngx_int_t  i;

    for (i = 0; i < n; i++) {
        /* 打开工作进程（ngx_worker_process_cycle 回调函数）*/
        ngx_spawn_process(ngx_worker_process_cycle, (void*)(intptr_t)i, "worker process");

    }
}

ngx_pid_t ngx_spawn_process(ngx_spawn_proc_pt proc, void *data, char *name)
{
    u_long     on;
    ngx_pid_t  pid;
    ngx_int_t  s;

    pid = fork();

    switch (pid) {
        case -1:
            printf("fork() failedn");
            return NGX_INVALID_PID;
        case 0:
            ngx_parent = getppid();
            ngx_pid = getpid();
            proc(data);
            break;
        default:
            break;
    }

    return pid;
}

/* 子进程回调函数
   每个进程的逻辑处理就从这个地方开始
 */
static void ngx_worker_process_cycle(void *data)
{
    ngx_int_t worker = (intptr_t) data;

    /* 工作进程初始化 */
    ngx_worker_process_init(worker);

    /* 进程循环 */
    for ( ;; ) {
        /*
            TODO 函数实现
         */
        // ngx_process_events_and_timers(cycle);
        sleep(1);
        printf("%d\n", (int)getpid());

        if (ngx_terminate) {
            ngx_worker_process_exit();
        }
    }
}

static void ngx_worker_process_init(ngx_int_t worker)
{
    sigset_t          set;
    ngx_int_t         n;
    ngx_uint_t        i;

    /* geteuid() 返回有效用户的ID*/
    /* 设置UID GROUPUID */
    /* TODO
    if (geteuid() == 0) {
        if (setgid(ccf->group) == -1) {
            printf("setgid failed\n");
            exit(2);
        }

        if (initgroups(ccf->username, ccf->group) == -1) {
            printf("initgroups failed\n");
        }

        if (setuid(ccf->user) == -1) {
            printf("setuid failed\n");
            exit(2);
        }
    }
    */

    if (chdir("/home/lixujun/tmp") == -1) {
        printf("chdir failed");
        exit(2);
    }

    sigemptyset(&set);
    /* 清除所有信号 */
    if (sigprocmask(SIG_SETMASK, &set, NULL) == -1) {
        printf("sigprocmask failed");
    }
}

static void ngx_worker_process_exit()
{
    printf("exit\n");
    exit(0);
}
