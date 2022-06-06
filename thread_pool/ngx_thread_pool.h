#ifndef _NGX_THREAD_POOL_H_INCLUDED_
#define _NGX_THREAD_POOL_H_INCLUDED_

#include "ngx_typedef.h"
#include "ngx_core.h"

struct ngx_thread_task_s {
    ngx_thread_task_t   *next;
    ngx_uint_t           id;
    void                *ctx;
    void               (*handler)(void *data);
};

typedef struct ngx_thread_pool_s  ngx_thread_pool_t;

ngx_thread_task_t *ngx_thread_task_alloc(ngx_pool_t *pool, size_t size);
ngx_int_t ngx_thread_task_post(ngx_thread_pool_t *tp, ngx_thread_task_t *task);

#endif /* _NGX_THREAD_POOL_H_INCLUDED_ */
