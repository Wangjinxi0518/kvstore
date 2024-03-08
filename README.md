# KV-Store

## KV 存储服务 项目简介

KV-Store 是⼀个底层使⽤ LevelDB 作为存储引擎，上层基于 Epoll 进⾏封装的⾼性能服务器，⽬前⾃⼰实现了如下
⼀些功能

* 完全兼容 Redis 协议，并且实现了⼀些基础命令 Get\Set\FlushAll\ShutDown 等等.
* 兼容了 Pipeline 的访问模式，有效减⼩了⽹络开销从⽽提升性能.
* 为了保证数据安全，⾃⼰实现了密码校验机制.
* 连接管理模块，⽀持多连接并发访问(通过限制连接单次读取数据量，避免了不同连接互相影响).
* 监控统计模块，⽀持 QPS， 连接数，出⼊流量等监控指标的展示.

## 项目结构

```
.
├── db
│   ├── 000005.log
│   ├── CURRENT
│   ├── LOCK
│   ├── LOG
│   ├── LOG.old
│   └── MANIFEST-000004
├── docs
│   └── README.md
├── dump.rdb
├── echo.txt
├── include
│   ├── conf.h
│   ├── csapp.h
│   ├── kv_cluster_epoll.h
│   ├── kv_command.h
│   ├── kv_conn.h
│   ├── kv_encode.h
│   ├── kv_exception.h
│   ├── kv_io.h
│   ├── serverstats.h
│   └── storage_engine.h
├── Makefile
├── src
│   ├── csapp.cc
│   ├── echoclient.cc
│   ├── echoserveri.cc
│   ├── kv_cluster_epoll.cc
│   ├── kv_command.cc
│   ├── kv_conn.cc
│   ├── kv_encode.cc
│   ├── kv_exception.cc
│   ├── kv_io.cc
│   ├── serverstats.cc
│   └── storage_engine.cc
└── test
    ├── echo.cc
    ├── echoservert.c
    ├── echoservert_origin.cpp
    ├── Encode.cpp
    ├── kv_leveldb.cc
    ├── kv_leveldb.h
    ├── sbuf.c
    ├── sbuf.h
    └── test.txt
```

### csapp.cc

"csapp.cc"是《Computer Systems: A Programmer's Perspective》一书中提供的一组用于简化网络编程的函数库。这些函数主要包括以下几类：

错误处理函数：例如unix_error和posix_error，它们用于在发生错误时打印错误信息并退出程序。

I/O函数：例如Rio_readn和Rio_writen，它们是对Unix I/O函数的包装，可以提供更安全、更方便的读写接口。

网络函数：例如Open_clientfd和Open_listenfd，它们用于打开客户端或服务器的网络连接。

进程控制函数：例如Fork，Execve等，它们是对Unix进程控制函数的包装，用于创建和管理进程。

内存管理函数：例如Malloc，Realloc等，它们是对Unix内存管理函数的包装，提供了错误检查功能。

信号处理函数：例如Signal，它是对Unix信号处理函数的包装，用于设置信号处理函数。

这些函数的目的是提供一个更高级别、更易于使用的接口，以简化网络编程的复杂性。


## TODO:

* 保证高可用性，实现主从机制
* 兼容更多 Redis 命令
* 单线程可能不够，考虑朝多线程发展(考虑线程安全问题，涉及锁机制)
* 编写配置文件，完善 Info 命令
 
