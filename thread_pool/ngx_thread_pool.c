#include "ngx_typedef.h"
#include "ngx_core.h"
#include "ngx_thread_pool.h"
#include <signal.h>

#define ngx_thread_pool_queue_init(q)                                         \
    (q)->first = NULL;                                                        \
    (q)->last = &(q)->first

ngx_thread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER;
static void ngx_thread_pool_destroy(ngx_thread_pool_t *tp);
static void ngx_thread_pool_exit_handler(void *data);

static void *ngx_thread_pool_cycle(void *data);
static void ngx_thread_pool_handler();

static ngx_str_t  ngx_thread_pool_default = ngx_string("default");

static ngx_uint_t               ngx_thread_pool_task_id;
static ngx_thread_pool_queue_t  ngx_thread_pool_done;     // 完成队列


ngx_int_t
ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_pool_t *pool)
{
    int             err;
    pthread_t       tid;
    ngx_uint_t      n;
    pthread_attr_t  attr;

    /* 任务队列初始化 */
    ngx_thread_pool_queue_init(&tp->queue);

    /* 互斥锁初始化 */
    if (ngx_thread_mutex_create(&tp->mtx) != NGX_OK) {
        return NGX_ERROR;
    }

    /* 条件变量初始化 */
    if (ngx_thread_cond_create(&tp->cond) != NGX_OK) {
        (void) ngx_thread_mutex_destroy(&tp->mtx);
        return NGX_ERROR;
    }

    /* 线程属性初始化 */
    err = pthread_attr_init(&attr);
    if (err) {
        printf("pthread_attr_init() failed");
        return NGX_ERROR;
    }

    /* 设置线程为分离属性 */
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err) {
        printf("pthread_attr_setdetachstate() failed");
        return NGX_ERROR;
    }

    for (n = 0; n < tp->threads; n++) {
        err = pthread_create(&tid, &attr, ngx_thread_pool_cycle, tp);
        if (err) {
            printf("pthread_create() failed");
            return NGX_ERROR;
        }
    }

    /* 销毁线程属性 */
    (void) pthread_attr_destroy(&attr);

    return NGX_OK;
}


static void
ngx_thread_pool_destroy(ngx_thread_pool_t *tp)
{
    ngx_uint_t           n;
    ngx_thread_task_t    task;
    volatile ngx_uint_t  lock;

    ngx_memzero(&task, sizeof(ngx_thread_task_t));    // #define ngx_memzero(buf, n)       (void) memset(buf, 0, n)

    task.handler = ngx_thread_pool_exit_handler;
    task.ctx = (void *) &lock;

    /* 向线程池的任务队列中添加tp->threads个任务（线程退出任务） */
    for (n = 0; n < tp->threads; n++) {
        lock = 1;

        if (ngx_thread_task_post(tp, &task) != NGX_OK) {
            return;
        }

        while (lock) {
            usleep(1);
        }

        task.active = 0;
    }

    /* 条件变量销毁 */
    (void) ngx_thread_cond_destroy(&tp->cond);

    /* 互斥锁销毁 */
    (void) ngx_thread_mutex_destroy(&tp->mtx);
}


static void
ngx_thread_pool_exit_handler(void *data)
{
    ngx_uint_t *lock = data;

    *lock = 0;

    pthread_exit(0);
}


ngx_thread_task_t *
ngx_thread_task_alloc(ngx_pool_t *pool, size_t size)
{
    ngx_thread_task_t  *task;

    task = ngx_pcalloc(pool, sizeof(ngx_thread_task_t) + size);
    if (task == NULL) {
        return NULL;
    }

    /* 线程上下文的起始地址 */
    task->ctx = task + 1;

    return task;
}


ngx_int_t
ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task)
{
    if (task->active) {
        printf("task #%ld already active", task->id);
        return NGX_ERROR;
    }

    /* 加锁 */
    if (ngx_thread_mutex_lock(&tp->mtx) != NGX_OK) {
        return NGX_ERROR;
    }

    /* 判断线程池是否已满 */
    if (tp->waiting >= tp->max_queue) {
        (void) ngx_thread_mutex_unlock(&tp->mtx);

        printf("thread pool %s queue overflow: %ld tasks waiting\n", tp->name.data, tp->waiting);
        return NGX_ERROR;
    }

    /* 激活任务 */
    task->active = 1;

    task->id = ngx_thread_pool_task_id++;                  // （全局静态变量）unsigned long int
    task->next = NULL;

    if (ngx_thread_cond_signal(&tp->cond) != NGX_OK) {
        (void) ngx_thread_mutex_unlock(&tp->mtx);
        return NGX_ERROR;
    }

    /* 将任务添加到任务队列的队尾 */
    *tp->queue.last = task;
    tp->queue.last = &task->next;

    tp->waiting++;

    (void) ngx_thread_mutex_unlock(&tp->mtx);

    printf("task #%ld added to thread pool \"%s\"", task->id, tp->name.data);

    return NGX_OK;
}

/* 线程处理函数 */
static void *
ngx_thread_pool_cycle(void *data)
{
    ngx_thread_pool_t *tp = data;

    int                 err;
    sigset_t            set;
    ngx_thread_task_t  *task;

    printf("thread in pool \"%s\" started\n", tp->name.data);

    sigfillset(&set);

    sigdelset(&set, SIGILL);
    sigdelset(&set, SIGFPE);
    sigdelset(&set, SIGSEGV);
    sigdelset(&set, SIGBUS);

    err = pthread_sigmask(SIG_BLOCK, &set, NULL);
    if (err) {
        printf("pthread_sigmask() failed\n");
        return NULL;
    }

    for ( ;; ) {
        if (ngx_thread_mutex_lock(&tp->mtx) != NGX_OK) {
            return NULL;
        }

        tp->waiting--;

        while (tp->queue.first == NULL) {
            if (ngx_thread_cond_wait(&tp->cond, &tp->mtx) != NGX_OK) {
                (void) ngx_thread_mutex_unlock(&tp->mtx);
                return NULL;
            }
        }

        task = tp->queue.first;
        tp->queue.first = task->next;

        if (tp->queue.first == NULL) {
            tp->queue.last = &tp->queue.first;
        }

        if (ngx_thread_mutex_unlock(&tp->mtx) != NGX_OK) {
            return NULL;
        }

        printf("run task #%ld in thread pool \"%s\"\n", task->id, tp->name.data);

        task->handler(task->ctx);

        printf("complete task #%ld in thread pool \"%s\"\n", task->id, tp->name.data);

        task->next = NULL;

        ngx_thread_mutex_lock(&thread_mutex);

        *ngx_thread_pool_done.last = task;
        ngx_thread_pool_done.last = &task->next;

        ngx_thread_mutex_unlock(&thread_mutex);

        ngx_thread_pool_handler();
    }
}

static void
ngx_thread_pool_handler()
{
    ngx_thread_task_t  *task;

    ngx_thread_mutex_lock(&thread_mutex);
    task = ngx_thread_pool_done.first;
    ngx_thread_pool_done.first = NULL;
    ngx_thread_pool_done.last = &ngx_thread_pool_done.first;
    ngx_thread_mutex_unlock(&thread_mutex);

    while (task) {
       printf("run completion handler for task #%ld\n", task->id);
        task->active = 0;
        task = task->next;
    }
}

ngx_thread_pool_t *
ngx_thread_pool_add(ngx_pool_t *pool, ngx_array_t *arrayPool, ngx_str_t *name)
{
    ngx_thread_pool_t       *tp, **tpp;
    if (name == NULL) {
        name = &ngx_thread_pool_default;
    }

    tp = ngx_thread_pool_get(arrayPool, name);

    if (tp) {
        return tp;
    }

    tp = ngx_pcalloc(pool, sizeof(ngx_thread_pool_t));
    if (tp == NULL) {
        return NULL;
    }

    tp->name = *name;
    tp->file = NULL;

    tpp = ngx_array_push(arrayPool);
    if (tpp == NULL) {
        return NULL;
    }

    *tpp = tp;

    return tp;
}

ngx_thread_pool_t *
ngx_thread_pool_get(ngx_array_t *arrayPool, ngx_str_t *name)
{
    ngx_uint_t                i;
    ngx_thread_pool_t       *tp;

    for (i = 0; i < arrayPool->nelts; i++) {
        tp = (ngx_thread_pool_t*)(arrayPool->elts);
        if (tp->name.len == name->len
            && ngx_strncmp(tp->name.data, name->data, name->len) == 0) {
            return tp;
        }
    }

    return NULL;
}

void ngx_thread_pool_done_queue_init()
{
    ngx_thread_pool_queue_init(&ngx_thread_pool_done);
}

