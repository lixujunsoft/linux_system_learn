#ifndef _ERR_H_INCLUDED_
#define _ERR_H_INCLUDED_

extern int daemon_proc;

void err_ret(const char *fmt, ...);
void err_sys(const char *fmt, ...);
void err_dump(const char *fmt, ...);
void err_msg(const char *fmt, ...);
void err_quit(const char *fmt, ...);

#endif