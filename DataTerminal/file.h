#ifndef _FILE_H_INCLUDED_
#define _FILE_H_INCLUDED_

#include "ngx_typedef.h"
#include "ngx_core.h"

#define CloseFile(fd) close(fd)
int OpenFile(char *name);
ssize_t WriteFile(int fd, const void *buf, size_t count);

#endif