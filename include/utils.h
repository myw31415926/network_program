/**
 * desc: 公共方法，socket 基础函数...
 * file: utils.h
 *
 * author:  myw31415926
 * date:    20180930
 * version: V0.1
 *
 * the closer you look, the less you see
 */

#ifndef _UTILS_V01_H
#define _UTILS_V01_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>      /* read() write() fork() */
#include <time.h>        /* timespec for pselect() */
#include <sys/types.h>   /* basic system data types */
#include <sys/socket.h>  /* basic socket definitions */
#include <sys/time.h>    /* timeval for select() */
#include <netinet/in.h>  /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>   /* inet(3) functions */

#include "log.h"

/* Miscellaneous constants */
#define MAXLINE     1024    /* max text line length */
#define BUFFSIZE    8192    /* buffer size for reads and writes */

/* *INDENT-OFF* */
#ifndef INET_ADDRSTRLEN
/* $$.Ic INET_ADDRSTRLEN$$ */
#define INET_ADDRSTRLEN     16  /* "ddd.ddd.ddd.ddd\0"
                                    1234567890123456 */
#endif

/* Define following even if IPv6 not supported, so we can always allocate
   an adequately sized buffer without #ifdefs in the code. */
#ifndef INET6_ADDRSTRLEN
/* $$.Ic INET6_ADDRSTRLEN$$ */
#define INET6_ADDRSTRLEN    46  /* max size of IPv6 address string:
                   "xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx" or
                   "xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:ddd.ddd.ddd.ddd\0"
                    1234567890123456789012345678901234567890123456 */
#endif

/* min and max */
#define min(a, b)    ((a) < (b) ? (a) : (b))
#define max(a, b)    ((a) > (b) ? (a) : (b))

/* 检测返回值，不等于则返回错误 */
#define CHECK_EQ_RETURN(ret, val, format, args...)  \
    if (ret != val) {                               \
        log_err(format, ##args);                    \
        return ret;                                 \
    }

/* 检测返回值，小于则返回错误 */
#define CHECK_LG_RETURN(ret, val, format, args...)  \
    if (ret < val) {                                \
        log_err(format, ##args);                    \
        return ret;                                 \
    }


/* 从socket中读取n个字节，返回实际读取的字节数 */
ssize_t read_n(int fd, void *buf, size_t nbytes);

/* 向socket中写入n个字节，返回实际写入的字节数 */
ssize_t write_n(int fd, void *buf, size_t nbytes);

/* 从socket中读取一行数据，返回实际读取的字节数 */
ssize_t read_line(int fd, void *buf, size_t maxlen);

#endif /*_UTILS_V01_H */
