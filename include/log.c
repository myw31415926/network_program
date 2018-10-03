/**
 * desc: 日志打印
 * file: log.c
 *
 * author:  myw31415926
 * date:    20180930
 * version: V0.1
 *
 * the closer you look, the less you see
 */

#include "log.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>      /* strerror */
#include <stdarg.h>      /* ANSI C header file */
#include <syslog.h>      /* for syslog() */

#define LOG_MAX   1023   /* max log line length */

static int daemon_proc = 0;     /* set nonzero by log_daemon() */

/* msg output */
static void msg_doit(int errnoflag, int level, const char *fmt, va_list ap);

/* Print log into linux syslog when daemon = 1 */
void log_daemon(void)
{
    daemon_proc = 1;
}

/* Print log info message */
void log_info(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    msg_doit(0, LOG_INFO, fmt, ap);
    va_end(ap);
}

/* Print debug message */
void log_debug(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    msg_doit(0, LOG_DEBUG, fmt, ap);
    va_end(ap);
}

/* Print error message with errno */
void log_err(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    msg_doit(1, LOG_ERR, fmt, ap);
    va_end(ap);
}

/* Print message with errno and exit(terminate) */
void log_err_quit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    msg_doit(1, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}

/* Print message with errno and coredump */
void log_err_dump(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    msg_doit(1, LOG_ERR, fmt, ap);
    va_end(ap);
    abort();        /* dump core and terminate */
    exit(1);        /* shouldn't get here */
}

/* Print message and return to caller */
static void msg_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
    int  errno_save, n;
    char buf[LOG_MAX + 1];

    errno_save = errno;     /* value caller might want printed */
    vsnprintf(buf, LOG_MAX, fmt, ap);   /* safe */

    n = strlen(buf);
    if (errnoflag & errno_save)
        snprintf(buf + n, LOG_MAX - n, ": %s", strerror(errno_save));
    strcat(buf, "\n");

    if (daemon_proc) {
        syslog(level, buf);
    } else {
        fflush(stdout);     /* in case stdout and stderr are the same */

        if (LOG_ERR == level)
            fputs("[ERROR] ", stderr);
        else if (LOG_DEBUG == level)
            fputs("[DEBUG] ", stderr);
        else
            fputs("[INFO ] ", stderr);

        fputs(buf, stderr);
        fflush(stderr);
    }
}

