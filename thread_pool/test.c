#include "ngx_typedef.h"
#include "ngx_core.h"
#include "ngx_thread_pool.h"
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

int main()
{
    ngx_pool_t *memPool = ngx_create_pool(16 * 1024);

    ngx_array_t *arrayPool = ngx_array_create(memPool, 10, sizeof(ngx_thread_pool_t));

    ngx_thread_pool_done_queue_init();
    ngx_str_t  threadStr = ngx_string("default");
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
    ngx_destroy_pool(memPool);
}