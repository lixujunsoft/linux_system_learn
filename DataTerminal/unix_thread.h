#ifndef _UNIX_THREAD_H_INCLUDED_
#define _UNIX_THREAD_H_INCLUDED_

int makethread(void *(*fn)(void *), void *arg);

#endif