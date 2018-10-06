/**
 * desc: TCP demo程序，TCP Server使用fork处理一个TCP Client请求，统计客户端数据
 * file: data_srv.c
 *
 * author:  myw31415926
 * date:    20181005
 * version: V0.1
 *
 * the closer you look, the less you see
 */

#include "utils.h"

#define SERV_PORT       8000
#define LISTENQ         16

/* 处理SIGCHLD信号 */
void sig_chld(int signo)
{
    pid_t pid;
    int   stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        log_debug("child %d terminated\n", pid);
    return;
}

/* 接收消息，并回射给客户端 */
void echo_srv(int sockfd, struct sockaddr_in *cliaddr)
{
    ssize_t n;
    char    ip[INET_ADDRSTRLEN];
    char    buf[MAXLINE];

    while (1) {
        n = read(sockfd, buf, MAXLINE);
        if (n > 0) {
            buf[n] = 0;
            inet_ntop(AF_INET, &(cliaddr->sin_addr), ip, INET_ADDRSTRLEN);
            log_debug("recv client[%s:%d]: %s", ip, ntohs(cliaddr->sin_port), buf);

            write_n(sockfd, buf, n);
        } else if (n < 0) {
            if (errno == EINTR) continue;   /* 忽略中断错误 */

            log_err_quit("str_echo: read error!");
        } else {
            log_debug("str_echo recv none, exit!");
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    int ret, listenfd, connfd;
    pid_t child_pid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, srvaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_LG_RETURN(listenfd, 0, "create socket failed!");

    bzero(&srvaddr, sizeof(srvaddr));
    srvaddr.sin_family      = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr.sin_port        = htons(SERV_PORT);
    ret = bind(listenfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr));
    CHECK_EQ_RETURN(ret, 0, "bind socket failed!");

    ret = listen(listenfd, LISTENQ);
    CHECK_EQ_RETURN(ret, 0, "listen socket failed!");

    signal(SIGCHLD, sig_chld);      /* 注册SIGCHLD信号，避免子进程僵死 */

    while (1) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
        CHECK_LG_RETURN(connfd, 0, "accept socket failed!");

        if (0 == (child_pid = fork())) {    /* child process */
            close(listenfd);    /* close listening socket */
            echo_srv(connfd, &cliaddr);   /* process the request */
            exit(0);
        }
        close(connfd);          /* parent close connected socket */
    }

    return 0;
}
