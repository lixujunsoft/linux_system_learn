#ifndef _UNIX_PROCESS_H_INCLUDED_
#define _UNIX_PROCESS_H_INCLUDED_

#include "ngx_typedef.h"
#include "ngx_core.h"

#define NGX_INVALID_PID  -1
typedef void (*ngx_spawn_proc_pt) (void *data);
typedef pid_t       ngx_pid_t;

void ngx_master_process_cycle(ngx_int_t n);

#endif