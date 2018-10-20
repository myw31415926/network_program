/**
 * desc: 公共方法，socket 基础函数...
 * file: utils.c
 *
 * author:  myw31415926
 * date:    20180930
 * version: V0.1
 *
 * the closer you look, the less you see
 */

#include "utils.h"

#define BINARY_LINE_MAX     16

/* 从socket中读取n个字节，返回实际读取的字节数 */
ssize_t read_n(int fd, void *buf, size_t nbytes)
{
    ssize_t nread;
    size_t  nleft = nbytes;
    char    *pbuf = buf;

    while (nleft > 0) {
        nread = read(fd, pbuf, nleft);
        if (nread < 0) {
            if (errno == EINTR)
                nread = 0;      /* call read() again */
            else
                return -1;      /* read error */
        } else if (nread == 0)
            break;              /* EOF */

        nleft -= nread;
        pbuf  += nread;
    }

    return (nbytes - nleft);
}

/* 向socket中写入n个字节，返回实际写入的字节数 */
ssize_t write_n(int fd, void *buf, size_t nbytes)
{
    ssize_t nwrite;
    size_t  nleft = nbytes;
    char    *pbuf = buf;

    while (nleft > 0) {         /* quit while until write over, nleft == 0 */
        nwrite = write(fd, pbuf, nleft);
        if (nwrite <= 0) {
            if (nwrite < 0 && errno == EINTR)
                nwrite = 0;     /* call write() again */
            else
                return -1;      /* write error */
        } else if (nwrite == 0)
            break;              /* EOF */

        nleft -= nwrite;
        pbuf  += nwrite;
    }

    return (nbytes);
}

/* 读取一个字符到buf中，不需要每次都调用read */
static ssize_t _read_char(int fd, char *pchar)
{
    static int  read_cnt = 0;
    static char read_buf[1024];
    static char *read_ptr = NULL;

    while (read_cnt == 0) { /* buf 中没有数据，需要再次从 socket 读取 */
        read_cnt = read(fd, read_buf, sizeof(read_buf));
        if (read_cnt < 0) {
            if (errno == EINTR)
                continue;
            else
                return -1;
        } else if (read_cnt == 0)
            return 0;       /* EOF */

        read_ptr = read_buf;
    }

    read_cnt--;
    *pchar = *read_ptr++;

    return 1;
}

/* 从socket中读取一行数据，返回实际读取的字节数 */
ssize_t read_line(int fd, void *buf, size_t maxlen)
{
    ssize_t n, rc = 0;
    char    c, *pchar;

    pchar = buf;
    for (n = 1; n < maxlen; n++) {
        if ((rc = _read_char(fd, &c)) == 1) {
            *pchar++ = c;
            if (c == '\n')
                break;      /* newline is over */
        } else if (rc == 0) {
            *pchar = 0;
            return (n - 1); /* EOF, read n - 1 bytes */
        } else 
            return -1;
    }

    *pchar = 0;             /* null terminate */
    return n;
}

/* 产生0~1之间的随机数 */
static inline double _inline_rand_1()
{
    return rand() / (RAND_MAX + 1.0);
}

/* 产生0~1之间的随机数，需要先srand()初始化随机种子 */
double rand_1()
{
    return _inline_rand_1();
}

/* 产生0~n之间的随机整数，需要先srand()初始化随机种子 */
int rand_n(int n)
{
    return (int)(n * _inline_rand_1());
}

/* 打印buf中的二进制数据，每行显示16个数据 */
void print_binary_data(unsigned char *buf, size_t size)
{
    int i;

    for (i = 0; i < size; i++) {
        if (i % BINARY_LINE_MAX == 0) fprintf(stdout, "%-8d| ", (i / BINARY_LINE_MAX) + 1);
        if (i % BINARY_LINE_MAX == (BINARY_LINE_MAX >> 1)) fprintf(stdout, "   ");
        fprintf(stdout, "%02x ", buf[i]);
        if (i % BINARY_LINE_MAX == (BINARY_LINE_MAX - 1)) fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}
