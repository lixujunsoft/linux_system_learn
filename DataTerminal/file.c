#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "ngx_typedef.h"
#include "ngx_core.h"

int OpenFile(char *name)
{
    int fd;

    fd = open((const char *) name, O_CREAT|O_EXCL|O_RDWR, 0600);

    if (-1 == fd) {
        if (errno != EEXIST) {
            return RET_ERROR;
        }
        fd = open((const char *) name, O_RDWR, 0600);
    }

    return fd;
}

ssize_t WriteFile(int fd, const void *buf, size_t count)
{
    if (NULL == buf || 0 == count) {
        return RET_ERROR;
    }
    if (write(fd, buf, count) != count) {
        perror("WriteFile");
        return RET_ERROR;
    }
    return RET_OK;
}