#ifndef _NGX_THREAD_H_INCLUDED_
#define _NGX_THREAD_H_INCLUDED_

#include "ngx_typedef.h"
#include "ngx_core.h"
#include <pthread.h>

typedef pthread_mutex_t  ngx_thread_mutex_t;
typedef pthread_cond_t  ngx_thread_cond_t;

ngx_int_t ngx_thread_mutex_create(ngx_thread_mutex_t *mtx);
ngx_int_t ngx_thread_mutex_destroy(ngx_thread_mutex_t *mtx);
ngx_int_t ngx_thread_mutex_lock(ngx_thread_mutex_t *mtx);
ngx_int_t ngx_thread_mutex_unlock(ngx_thread_mutex_t *mtx);


ngx_int_t ngx_thread_cond_create(ngx_thread_cond_t *cond);
ngx_int_t ngx_thread_cond_destroy(ngx_thread_cond_t *cond);
ngx_int_t ngx_thread_cond_signal(ngx_thread_cond_t *cond);
ngx_int_t ngx_thread_cond_wait(ngx_thread_cond_t *cond, ngx_thread_mutex_t *mtx);

#endif
