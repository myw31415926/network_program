/**
 * desc: 日志打印
 * file: log.h
 *
 * author:  myw31415926
 * date:    20180930
 * version: V0.1
 *
 * the closer you look, the less you see
 */

#ifndef _MSG_LOG_H
#define _MSG_LOG_H

/* Print log into linux syslog */
void log_daemon(void);

/* Print log info message */
void log_info(const char *fmt, ...);

/* Print debug message */
void log_debug(const char *fmt, ...);

/* Print error message with errno */
void log_err(const char *fmt, ...);

/* Print message without errno and exit(terminate) */
void log_err_quit(const char *fmt, ...);

/* Print message with errno and coredump */
void log_err_dump(const char *fmt, ...);

#endif /* _MSG_LOG_H */
