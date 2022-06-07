#ifndef _NGX_THREAD_POOL_H_INCLUDED_
#define _NGX_THREAD_POOL_H_INCLUDED_

#include "ngx_typedef.h"
#include "ngx_core.h"

struct ngx_thread_task_s {
    ngx_thread_task_t   *next;
    ngx_uint_t           id;
    void                *ctx;
    void               (*handler)(void *data);
    ngx_uint_t          active;
};

typedef struct {
    ngx_thread_task_t        *first;
    ngx_thread_task_t       **last;
} ngx_thread_pool_queue_t;

struct ngx_thread_pool_s {
    ngx_thread_mutex_t        mtx;
    ngx_thread_pool_queue_t   queue;                 // 任务队列
    ngx_int_t                 waiting;
    ngx_thread_cond_t         cond;

    ngx_str_t                 name;
    ngx_uint_t                threads;               // 线程池中的线程数量
    ngx_int_t                 max_queue;

    u_char                   *file;
};

typedef struct ngx_thread_pool_s  ngx_thread_pool_t;

ngx_thread_task_t *ngx_thread_task_alloc(ngx_pool_t *pool, size_t size);
ngx_int_t ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task);

ngx_thread_pool_t *ngx_thread_pool_add(ngx_pool_t *pool, ngx_array_t *arrayPool,ngx_str_t *name);
ngx_thread_pool_t *ngx_thread_pool_get(ngx_array_t *arrayPool, ngx_str_t *name);
ngx_int_t ngx_thread_pool_init(ngx_thread_pool_t *tp, ngx_pool_t *pool);
void ngx_thread_pool_done_queue_init();

#endif /* _NGX_THREAD_POOL_H_INCLUDED_ */
