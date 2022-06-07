#include "ngx_typedef.h"
#include "ngx_core.h"
#include "ngx_thread_pool.h"

int main()
{
    ngx_pool_t *memPool = ngx_create_pool(16 * 1024);

    ngx_array_t *arrayPool = ngx_array_create(memPool, 10, sizeof(ngx_thread_pool_t));

    ngx_str_t  threadStr = ngx_string("default");
    ngx_thread_pool_t *threadPool = ngx_thread_pool_add(memPool, arrayPool, &threadStr);
    threadPool->threads = 10;
    ngx_thread_pool_init(threadPool, memPool);

    ngx_destroy_pool(memPool);
}