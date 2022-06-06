#include <stdio.h>
#include "ngx_palloc.h"

int main()
{
    ngx_pool_t *memPool = ngx_create_pool(1024);

    ngx_palloc(memPool, 100);
    ngx_palloc(memPool, 100);
    ngx_palloc(memPool, 1000);
    ngx_palloc(memPool, 10000);

    ngx_destroy_pool(memPool);
}