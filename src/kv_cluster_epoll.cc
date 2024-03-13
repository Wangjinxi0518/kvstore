/**
 * @File Name: kv_cluster_epoll.cc
 * @brief: 这段代码是一个名为ClusterEpoll的类的实现，该类封装了Linux的Epoll事件驱动模型的相关操作。Epoll是Linux下多路复用IO接口select/poll的增强版本，它能显著提高程序在大量并发连接中只有少量活跃的情况下的系统CPU利用率。
 * @Author : wangjinxi email:1023026823@qq.com
 *
 */
#include "kv_cluster_epoll.h"

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <glog/logging.h>

#define OPEN_MAX  100
#define SERV_PORT 5000
#define INFTIM    1000

int sockfd, epfd, nfd;
struct epoll_event ev, events[20];

void ClusterEpoll::EpollInit(int listenfd) {
    epfd = epoll_create(256);
    ev.data.fd = listenfd;    // 设置与要处理的事件相关的文件描述符
    ev.events = EPOLLIN;    // 设置要处理的事件类型
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);    // 注册epoll事件
}

void ClusterEpoll::SetInit(int connfd) {
    ev.data.fd = connfd;    // 设置用于读操作的文件描述符
    ev.events = EPOLLIN;    //设置用于注册的读操作事件
    epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);    // 注册ev
}

void ClusterEpoll::SetRead(int sockfd) {
    ev.data.fd = sockfd;    // 设置用于读操作的文件描述符
    ev.events = EPOLLIN;    // 修改sockfd上要处理的事件为EPOLIN
    epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
}

void ClusterEpoll::SetWrite(int sockfd) {
    ev.data.fd = sockfd;    // 设置用于写操作的文件描述符
    ev.events = EPOLLOUT;    //设置sockfd上要处理的事件为EPOLLOT
    epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
}

void ClusterEpoll::SetMiss(int i) {
    events[i].data.fd = -1;
}

bool ClusterEpoll::JudgeFirst(int i, int listenfd) {
    if (events[i].data.fd == listenfd) {
        return true;
    } else {
        return false;
    }
}

bool ClusterEpoll::JudgeRead(int i) {
    if (events[i].events & EPOLLIN) {
        return true;
    } else {
        return false;
    }
}

bool ClusterEpoll::JudgeWrite(int i) {
    if (events[i].events & EPOLLOUT) {
        return true;
    } else {
        return false;
    }
}

int ClusterEpoll::GetFD(int i) {
    return events[i].data.fd;
}

int ClusterEpoll::WaitEpoll() {
    return epoll_wait(epfd, events, 20, 100);
}
