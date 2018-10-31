/**
 * desc: I/O复用 demo程序，Server端，调用poll 回显字符串。
 * file: poll_echo_srv.c
 *
 * author:  myw31415926
 * date:    20181031
 * version: V0.1
 *
 * the closer you look, the less you see
 */

#include "utils.h"
#include <limits.H>             /* for OPEN_MAX */

#define SERV_PORT       8000
#define LISTENQ         16

int main(int argc, char const *argv[])
{
    int     ret;
    int     i, maxi, listenfd, connfd, sockfd;
    int     nready;
    ssize_t n;
    char    buf[MAXLINE];
    char    ip[INET_ADDRSTRLEN];
    socklen_t          clilen;
    struct sockaddr_in cliaddr, srvaddr;

    /**
     *  struct pollfd {
     *      int   fd;       // 要检测的描述符
     *      short events;   // 描述符指定的事件
     *      short revents;  // 返回描述符的状态
     *  }
     */
    struct pollfd      clients[OPEN_MAX];   /* client poll 事件 */

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

    /* 初始化监听 socket 事件 */
    clients[0].fd     = listenfd;
    clients[0].events = POLLRDNORM;     /* 普通数据可读 */
    for (i = 1; i < OPEN_MAX; i++)
        clients[i] = -1;                /* -1 表示未使用 */
    maxi = 0;                           /* clients 最大索引 */

    /* 轮询所有注册的socket */
    while (1) {
        nready = poll(clients, maxi + 1, INFTIM);    /* 检测读数据集，永远等待 */
        CHECK_LG_RETURN(nready, 0, "poll socket failed!");

        if (clients[0].revents & POLLRDNORM) {    /* 有新的连接 */
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
            CHECK_LG_RETURN(connfd, 0, "accept socket failed!");

            /* 保存client socket到最小的下标中 */
            for (i = 1; i < OPEN_MAX; i++)
                if (clients[i].fd < 0) {
                    clients[i].fd = connfd;
                    break;
                }
            if (OPEN_MAX == i)
                log_err_quit("poll_echo_srv: too many clients, count[%d]\n", i);

            clients[i].events = POLLRDNORM;
            if (i > maxi)
                maxi = i;               /* clients 数组最大索引 */

            if (--nready <= 0)
                continue;               /* 没有可读取的描述符 */
        }

        for (i = 1; i < maxi; i++) {    /* 检测所有 client 是否有数据 */
            if ((sockfd = clients[i].fd) < 0)
                continue;
            if (clients[i].revents & (POLLRDNORM | POLLERR)) {  /* 可读或错误 */
                n = read(sockfd, buf, MAXLINE)
                if (0 < n) {            /* 读取到数据 */
                    buf[n] = 0;
                    clilen = sizeof(cliaddr);
                    getsockname(sockfd, (struct sockaddr*)&cliaddr, &clilen)
                    inet_ntop(AF_INET, &(cliaddr->sin_addr), ip, INET_ADDRSTRLEN);
                    log_debug("recv client[%s:%d]: %s", ip, ntohs(cliaddr->sin_port), buf);

                    write_n(sockfd, buf, n);
                } else if (0 == n) {    /* client 断开连接 */
                    log_debug("poll_echo_srv: client close");
                    close(sockfd);
                    clients[i].fd = -1;
                } else {                /* 错误 */
                    if (errno == ECONNRESET) {
                        /* client 重置连接 */
                        close(sockfd);
                        clients[i].fd = -1;
                    } else {
                        log_err_quit("poll_echo_srv: read erroe, exit!");
                    }
                }
            }

            if (--nready <= 0)
                break;                 /* 没有可读取的描述符 */
        }
    }

    return 0;
}
