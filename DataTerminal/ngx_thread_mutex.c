#include "ngx_typedef.h"
#include "ngx_core.h"

ngx_int_t
ngx_thread_mutex_create(ngx_thread_mutex_t *mtx)
{
    ngx_err_t            err;
    pthread_mutexattr_t  attr;

    err = pthread_mutexattr_init(&attr);
    if (err != 0) {
        printf("pthread_mutexattr_init() failed");
        return NGX_ERROR;
    }

    err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    if (err != 0) {
        printf("(PTHREAD_MUTEX_ERRORCHECK) failed");
        return NGX_ERROR;
    }

    err = pthread_mutex_init(mtx, &attr);
    if (err != 0) {
        printf("pthread_mutex_init() failed");
        return NGX_ERROR;
    }

    err = pthread_mutexattr_destroy(&attr);
    if (err != 0) {
        printf("pthread_mutexattr_destroy() failed");
    }

    return NGX_OK;
}


ngx_int_t
ngx_thread_mutex_destroy(ngx_thread_mutex_t *mtx)
{
    ngx_err_t  err;

    err = pthread_mutex_destroy(mtx);
    if (err != 0) {
        printf("pthread_mutex_destroy() failed");
        return NGX_ERROR;
    }

    return NGX_OK;
}


ngx_int_t
ngx_thread_mutex_lock(ngx_thread_mutex_t *mtx)
{
    ngx_err_t  err;

    err = pthread_mutex_lock(mtx);
    if (err == 0) {
        return NGX_OK;
    }

    printf("pthread_mutex_lock() failed");

    return NGX_ERROR;
}


ngx_int_t
ngx_thread_mutex_unlock(ngx_thread_mutex_t *mtx)
{
    ngx_err_t  err;

    err = pthread_mutex_unlock(mtx);

    if (err == 0) {
        return NGX_OK;
    }

    printf("pthread_mutex_unlock() failed");

    return NGX_ERROR;
}
