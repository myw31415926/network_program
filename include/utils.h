/**
 * desc: 公共方法，日志打印，socket 基础函数...
 * file: util.h
 *
 * author: myw31415926
 * the closer you look, the less you see
 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>

/* 从socket中读取n个字节，返回实际读取的字节数 */
ssize_t read_n(int fd, void *buf, size_t nbytes);

/* 向socket中写入n个字节，返回实际写入的字节数 */
ssize_t write_n(int fd, void *buf, size_t nbytes);

/* 从socket中读取一行数据，返回实际读取的字节数 */
ssize_t read_line(int fd, void *buf, size_t maxlen);
