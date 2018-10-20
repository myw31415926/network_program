/**
 * desc: TCP demo程序，Client端发送数据，并接收Server端的计算结果
 * file: data_srv.c
 *
 * author:  myw31415926
 * date:    20181014
 * version: V0.1
 *
 * the closer you look, the less you see
 */

#include "utils.h"
#include "data_def.h"

/* 随机生成一个运算符，并构造操作数，发送Server端，等待结果 */
void data_cli(FILE *fp, int sockfd)
{
    ssize_t   n;
    argdata_s args;
    result_s  result;
    char      buf[MAXLINE];

    while (NULL != fgets(buf, MAXLINE, fp)) {
        if (2 != sscanf(buf, "%ld %ld", &args.arg1, &args.arg2)) {
            log_err("invalid input: %s", buf);
            continue;
        }

        args.arg1 = hton64(args.arg1);
        args.arg2 = hton64(args.arg2);
        write_n(sockfd, &args, sizeof(args));

        /* 接收Server端的计算结果 */
        n = read_n(sockfd, &result, sizeof(result));
        if (n > 0) {
            fprintf(stdout, "%ld\n", ntoh64(result.sum));
        } else if (n < 0) {
            if (errno == EINTR) continue;   /* 忽略中断错误 */
            log_err_quit("data_cli: read error!");
        } else {
            log_err_quit("echo_cli: server terminated prematurely");
        }
    }
}

int main(int argc, char *argv[])
{
    int ret, sockfd;
    struct sockaddr_in srvaddr;

    if (argc != 3) {
        log_err_quit("usage: %s ip port", argv[0]);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_LG_RETURN(sockfd, 0, "create socket failed!");

    bzero(&srvaddr, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port   = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &srvaddr.sin_addr);

    ret = connect(sockfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr));
    CHECK_EQ_RETURN(ret, 0, "connect socket failed!");

    data_cli(stdin, sockfd);

    exit(0);
}
