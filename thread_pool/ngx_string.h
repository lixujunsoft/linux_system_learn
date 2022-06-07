#ifndef _NGX_STRING_H_INCLUDED_
#define _NGX_STRING_H_INCLUDED_

#include "ngx_typedef.h"
#include "ngx_core.h"

#define ngx_memcpy(dst, src, n)   (void) memcpy(dst, src, n)

typedef struct {
    size_t      len;
    u_char     *data;
} ngx_str_t;

#endif