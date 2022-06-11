OBJ=test

OBJS += ngx_alloc.c
OBJS += ngx_array.c 
OBJS += ngx_palloc.c 
OBJS += ngx_thread_cond.c 
OBJS += ngx_thread_mutex.c 
OBJS += ngx_thread_pool.c 
OBJS += test.c
OBJS += msg_queue.c
OBJS += share_mem.c
OBJS += sem_array.c

cc=gcc

$(OBJ):$(OBJS)
	$(CC) $^ -o $@ -g
