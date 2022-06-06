#ifndef _NGX_ALLOC_H_INCLUDED_
#define _NGX_ALLOC_H_INCLUDED_

#include "ngx_typedef.h"
#include "ngx_core.h"

void *ngx_alloc(size_t size);
void *ngx_calloc(size_t size);

#define ngx_free          free

void *ngx_memalign(size_t alignment, size_t size);

#endif /* _NGX_ALLOC_H_INCLUDED_ */
