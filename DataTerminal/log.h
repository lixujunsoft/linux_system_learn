#ifndef _LOG_H_INCLUDED_
#define _LOG_H_INCLUDED_

#include "ngx_typedef.h"
#include "ngx_core.h"

int LogInit(char *pathName);
int LogDestroy();
void AddDebugLog(char *debugLogInfo);
void *WriteLogToFile(void *data);

#endif