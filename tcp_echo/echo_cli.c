/**
 * desc: TCP demo程序，TCP Server使用fork处理一个TCP Client请求，回显字符串。
 * file: echo_cli.c
 *
 * author:  myw31415926
 * date:    20180930
 * version: V0.1
 *
 * the closer you look, the less you see
 */

#include "utils.h"

/* 从stdin读一行文本，发送到服务器，并从服务器回射 */
void echo_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        write_n(sockfd, sendline, strlen(sendline));

        if (0 == read_line(sockfd, recvline, MAXLINE))
            log_err_quit("echo_cli: server terminated prematurely");

        fputs(recvline, stdout);
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
    CHECK_NE_RETURN(ret, 0, "connect socket failed!");

    echo_cli(stdin, sockfd);

    exit(0);
}
