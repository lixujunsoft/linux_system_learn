#include "ngx_typedef.h"
#include "ngx_core.h"
#include "ngx_thread_pool.h"
#include <sys/ipc.h>

#define READ 0
#define WRITE 1

typedef struct {
    int cmd;
    int data;
} task_data;

void deal_with_task_data(void *data)
{
    task_data *taskData = (task_data*)data;
    printf("cmd = %d\n", taskData->cmd);
    printf("cmd = %d\n", taskData->data);
}

/* 线程池测试 */
void ThreadpoolTest()
{
    ngx_pool_t *memPool = ngx_create_pool(16 * 1024);

    ngx_array_t *arrayPool = ngx_array_create(memPool, 10, sizeof(ngx_thread_pool_t));

    ngx_thread_pool_done_queue_init();
    ngx_str_t  threadStr = ngx_string("handle data");
    ngx_thread_pool_t *threadPool = ngx_thread_pool_add(memPool, arrayPool, &threadStr);
    threadPool->threads = 10;
    ngx_thread_pool_init(threadPool, memPool);

    /* 往工作队列中添加任务 */
    task_data taskData;
    taskData.cmd = 1;
    taskData.data = 8888;
    ngx_thread_task_t *task = ngx_thread_task_alloc(memPool, sizeof(task_data));
    task->ctx = &taskData;
    task->handler = deal_with_task_data;
    ngx_thread_task_post(threadPool, task);
    sleep(10);
    ngx_thread_pool_destroy(threadPool);
    sleep(5);
    ngx_array_destroy(arrayPool);
    ngx_destroy_pool(memPool);
}

/* 信号量数组和共享内存测试 */
void SemAndShmTest()
{
    pid_t pid;
    SemArrayInfo *semArrayInfo;
    ShmInfo *shmInfo;
    int semArrayInitValue[] = {0, 1};


    /* 信号量数组初始化 */
    semArrayInfo = SemInfoConstruct("/tmp", 'a', 2, IPC_CREAT | 0666 | IPC_EXCL);
    memmove(semArrayInfo->semArrayInitValue, semArrayInitValue, sizeof(semArrayInitValue));
    SemArrayInit(semArrayInfo);

    /* 共享内存初始化 */
    shmInfo = ShmInfoConstruct("/tmp", 'a', 1024, IPC_CREAT | 0666);
    ShmInit(shmInfo);

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    } else if (pid == 0) {

        while (1) {
            SemArray_P(semArrayInfo, READ);
            printf("child: %s\n", (char*)shmInfo->shmaddr);
            if (!strcmp("quit", shmInfo->shmaddr)) {
                printf("child process will exit\n");
                SemArrayDestroy(semArrayInfo);
                ShmDestroy(shmInfo);
                goto __ERROR;
            } 
            SemArray_V(semArrayInfo, WRITE);
        }
    } else {
        int i = 0;
        while (1) {
            sleep(2);
            SemArray_P(semArrayInfo, WRITE);
            printf("input >\n");
            sprintf(shmInfo->shmaddr, "hello world");
            if (i == 10) {
                sprintf(shmInfo->shmaddr, "quit");
                SemArray_V(semArrayInfo, READ);
                goto __ERROR;;
            }
            SemArray_V(semArrayInfo, READ);
            i++;
        }
    }
__ERROR:
}



int main()
{
    // ThreadpoolTest();
    SemAndShmTest();
}