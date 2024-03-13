/**
 * @File Name: kv_exception.cc
 * @brief 这段代码的主要是C++的异常处理。它解决了如何检查文件描述符和I/O操作是否有效的问题。
 * @Author : wangjinxi email:1023026823@qq.com
 *
 */
#include "kv_exception.h"

#include <glog/logging.h>

bool FDException(int fd) {
    return fd < 0;
}

bool IOException(int n) {
    return n < 0;
}
