#ifndef _NGX_ALLOC_H_INCLUDED_
#define _NGX_ALLOC_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>


void *ngx_alloc(size_t size);
void *ngx_calloc(size_t size);

#define ngx_free          free

void *ngx_memalign(size_t alignment, size_t size);

#endif /* _NGX_ALLOC_H_INCLUDED_ */
