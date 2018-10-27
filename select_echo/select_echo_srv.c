/**
 * desc: I/O复用 demo程序，调用select 回显字符串。
 * file: select_echo_srv.c
 *
 * author:  myw31415926
 * date:    20181027
 * version: V0.1
 *
 * the closer you look, the less you see
 */

#include "utils.h"

#define SERV_PORT       8000
#define LISTENQ         16

int main(int argc, char const *argv[])
{
    int     ret;
    int     i, maxi, maxfd, listenfd, connfd, sockfd;
    int     nready, client[FD_SET_SIZE];
    ssize_t n;
    fd_set  rset, allset;
    char    buf[MAXLINE];
    socklen_t          clilen;
    struct sockaddr_in cliaddr, srvaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_LG_RETURN(listenfd, 0, "create socket failed!");

    bzero(&srvaddr, sizeof(srvaddr));
    srvaddr.sin_family      = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr.sin_port        = htons(SERV_PORT);
    ret = bind(listenfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr));
    CHECK_NE_RETURN(ret, 0, "bind socket failed!");

    ret = listen(listenfd, LISTENQ);
    CHECK_NE_RETURN(ret, 0, "listen socket failed!");

    return 0;
}
