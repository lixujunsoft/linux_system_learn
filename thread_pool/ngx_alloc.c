#include "ngx_typedef.h"
#include "ngx_core.h"

ngx_uint_t  ngx_pagesize = 4096;
ngx_uint_t  ngx_pagesize_shift;
ngx_uint_t  ngx_cacheline_size;

/* 封装了malloc函数*/
void *
ngx_alloc(size_t size)
{
    void  *p;

    p = malloc(size);
    if (p == NULL) {
        printf("malloc(%lu) failed", size);
    }

    return p;
}

/* 调用ngx_alloc方法，如果分配成，则调用ngx_memzero方法，将内存块设置为0
   #define ngx_memzero(buf, n) (void) memset(buf, 0, n)
*/
void *
ngx_calloc(size_t size)
{
    void  *p;

    p = ngx_alloc(size);

    if (p) {
        ngx_memzero(p, size);
    }

    return p;
}

void *
ngx_memalign(size_t alignment, size_t size)
{
    void  *p;
    int    err;

    err = posix_memalign(&p, alignment, size);

    if (err) {
        
        p = NULL;
    }

    return p;
}