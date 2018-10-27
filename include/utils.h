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
#include <unistd.h>         /* read() write() fork() */
#include <time.h>           /* timespec for pselect() */
#include <sys/types.h>      /* basic system data types */
#include <sys/socket.h>     /* basic socket definitions */
#include <sys/select.h>     /* select() */
#include <sys/time.h>       /* timeval for select() */
#include <netinet/in.h>     /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>      /* inet(3) functions */
#include <sys/wait.h>       /* wait() */
#include <poll.h>           /* poll() */
#include <stdint.h>         /* int8/16/32/64_t, uint8/16/32/64_t, htobe64, be64toh */

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
#define CHECK_NE_RETURN(ret, val, format, args...)  \
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

/* 函数的起始和结束标记，需要配对使用 */
#define LABEL_FUNC_START            \
    struct timeval tv;              \
    gettimeofday(&tv, NULL);        \
    fprintf(stdout, "### [%ld.%ld] test start: %s\n", tv.tv_sec, tv.tv_usec, __func__);

#define LABEL_FUNC_END              \
    gettimeofday(&tv, NULL);        \
    fprintf(stdout, "### [%ld.%ld] test end: %s\n\n", tv.tv_sec, tv.tv_usec, __func__);


/* 64位网络字节序转主机字节序 */
#define ntoh64  be64toh
/* 64位主机字节序转网络字节序 */
#define hton64  htobe64


/* 从socket中读取n个字节，返回实际读取的字节数 */
ssize_t read_n(int fd, void *buf, size_t nbytes);

/* 向socket中写入n个字节，返回实际写入的字节数 */
ssize_t write_n(int fd, void *buf, size_t nbytes);

/* 从socket中读取一行数据，返回实际读取的字节数 */
ssize_t read_line(int fd, void *buf, size_t maxlen);

/* 产生0~1之间的随机数，需要先srand()初始化随机种子 */
double rand_1();

/* 产生0~n之间的随机整数，需要先srand()初始化随机种子 */
int rand_n(int n);

/* 打印buf中的二进制数据，每行显示16个数据 */
void print_binary_data(unsigned char *buf, size_t size);

#endif /*_UTILS_V01_H */
