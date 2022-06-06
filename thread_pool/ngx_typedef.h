#ifndef _NGX_TYPEDEF_H_INCLUDED_
#define _NGX_TYPEDEF_H_INCLUDED_

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)
#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)
#define ngx_close_file           close
#define ngx_delete_file(name)    unlink((const char *) name)

#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)
#define ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

#define NGX_ALIGNMENT   sizeof(unsigned long)
#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6

#define NGX_INVALID_FILE         -1
#define NGX_FILE_ERROR           -1

#define ngx_errno                  errno
#define NGX_ENOENT                 ENOENT
#define ngx_inline      inline

typedef int                      ngx_fd_t;
typedef unsigned char __u_char;
typedef __u_char u_char;

typedef unsigned long int	uintptr_t;
typedef uintptr_t       ngx_uint_t;

typedef long int		intptr_t;
typedef intptr_t        ngx_int_t;

extern ngx_uint_t  ngx_pagesize;
extern ngx_uint_t  ngx_pagesize_shift;
extern ngx_uint_t  ngx_cacheline_size;

typedef int               ngx_err_t;

typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;
typedef struct ngx_pool_s            ngx_pool_t;
typedef struct ngx_thread_task_s     ngx_thread_task_t;
typedef struct ngx_pool_s            ngx_pool_t;

#endif /* _NGX_TYPEDEF_H_INCLUDED_ */