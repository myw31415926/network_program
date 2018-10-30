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
    int     nready, clients[FD_SET_SIZE];   /* 保存所有client socket id */
    ssize_t n;
    fd_set  rset, allset;                   /* 读数据集，所有数据集 */
    char    buf[MAXLINE];
    char    ip[INET_ADDRSTRLEN];
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

    /* 初始化 maxfd, maxi, clients */
    maxfd = listenfd;
    maxi  = -1;
    for (i = 0; i < FD_SET_SIZE; i++)
        clients[i] = -1;

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    /* 轮询所有注册的socket */
    while (1) {
        rset = allset;      /* 结构体赋值 */
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);    /* 检测读数据集，永远等待 */
        CHECK_NE_RETURN(nready, 0, "select socket failed!");

        if (FD_ISSET(listenfd, &rset)) {    /* 有新的连接 */
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
            CHECK_LG_RETURN(connfd, 0, "accept socket failed!");

            /* 保存client socket到最小的下标中 */
            for (i = 0; i < FD_SET_SIZE; i++)
                if (clients[i] < 0) {
                    clients[i] = connfd;
                    break;
                }
            if (FD_SET_SIZE == i)
                log_err_quit("select_echo_srv: too many clients, count[%d]\n", i);

            FD_SET(connfd, &allset);    /* 向数据集中加入新的 socket 描述符*/
            if (connfd > maxfd)
                maxfd = connfd;         /* for select */
            if (i > maxi)
                maxi = i;               /* clients 数组最大索引 */

            if (--nready <= 0)
                continue;               /* 没有可读取的描述符 */
        }

        for (i = 0; i < maxi; i++) {    /* 检测所有 client 是否有数据 */
            if ((sockfd = clients[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset)) {  /* 有连接可以读取数据 */
                n = read(sockfd, buf, MAXLINE)
                if (n < 0) {
                    buf[n] = 0;
                    inet_ntop(AF_INET, &(cliaddr->sin_addr), ip, INET_ADDRSTRLEN);
                    log_debug("recv client[%s:%d]: %s", ip, ntohs(cliaddr->sin_port), buf);

                    write_n(sockfd, buf, n);
                } else if (0 == n) {
                    /* client 断开连接 */
                    log_debug("select_echo_srv: client close");
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    clients[i] = -1;
                } else {
                    if (errno == EINTR) continue;   /* 忽略中断错误 */
                    log_err_quit("select_echo_srv: recv erroe, exit!");
                }
            }

            if (--nready <= 0)
                break;                 /* 没有可读取的描述符 */
        }
    }

    return 0;
}
