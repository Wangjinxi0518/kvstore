/**
 * @File Name: kv_encode.cc
 * @brief: 这段代码是一个名为Encode的类的实现，主要用于处理和解析客户端发送的字符串指令。
 * @Author : wangjinxi email:1023026823@qq.com
 *
 */
#include "kv_encode.h"
#include "csapp.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <assert.h>
#include <glog/logging.h>

int Encode::getCharLength(const char *p) {    // 获取客户端传来的字符串长度
    int cnt = 0;
    while (*p++ != '\0') {
        cnt++;
    }
    return cnt;
}

bool Encode::Judgestring(const char in[]) {
    if (in[0] == '*') {
        return true;
    }
    return false;
}

void Encode::Forwardsearch(const std::string &in, int *const cur_pos) {
    while (in[*cur_pos] != '$') {
        (*cur_pos)++;
    }
}

void Encode::Backsearch(const std::string &in, int *const cur_pos) {
    while (in[*cur_pos] != '$') {
        (*cur_pos)--;
    }
}

bool Encode::JudgeOrder(const std::string &in,
                        int *const cur_pos) {    // 判断是不是$开头
    if (*cur_pos < MAXLINE && in[*cur_pos] == '$') {
        (*cur_pos)++;
        return true;
    } else {
        return false;
    }
}

bool Encode::paramtertotal(
    const std::string &in,
    int *const cur_pos,
    int *const size) {    // 使字符串指令变成int类型的size
    std::string word = "";
    if (*cur_pos >= MAXLINE) {    // 表示读到$这里刚好读满
        Backsearch(in, cur_pos);
        return false;
    }

    while (in[*cur_pos] != '\r' && *cur_pos < MAXLINE) {
        if (in[*cur_pos] >= '0' && in[*cur_pos] <= '9') {
            word.push_back(in[*cur_pos]);
            (*cur_pos)++;
        } else {
            return false;
        }
    }
    if (*cur_pos >= MAXLINE) {    // 表示读数字部分时中间段读满了,ex: xxxx$123|456在3这里读满了
        Backsearch(in, cur_pos);
    }
    if (*cur_pos < MAXLINE && in[*cur_pos] == '\r' && *cur_pos + 1 < MAXLINE && in[*cur_pos + 1] == '\n') {
        *size = atoi(word.c_str());
        (*cur_pos) += 2;
        return true;
    } else {
        Backsearch(in,
                   cur_pos);    // 在数字部分刚好读满以及'\r'或'\n'这里中断读满的情况
        return false;
    }
}

bool Encode::Split(
    const std::string &in,
    std::vector<std::string> *const out,
    int *const cur_pos,
    int *const size) {    // 使key, value, 指令装到vector里面
    std::string str = "";
    if (*cur_pos >= MAXLINE) {    // 在刚开始读数据部分刚好读满
        Backsearch(in, cur_pos);
    }
    str.assign(in, *cur_pos, *size);
    int pos = *cur_pos;
    int Size = *size;
    if (pos + Size - 1 < MAXLINE && pos + Size < MAXLINE && in[pos + Size] == '\r' && pos + Size + 1 < MAXLINE
        && in[pos + Size + 1] == '\n') {
        out->push_back(str);
        (*cur_pos) = pos + Size + 2;
        return true;
    } else {
        Backsearch(in,
                   cur_pos);    // 在读数据中途读满，ex:$12\r\nqwer|asddxx在r这里读满
        return false;
    }
}

// 换成c++标准库函数
void Encode::orderTolower(std::string &order) {    // 指令小写化
    std::transform(order.begin(), order.end(), order.begin(), ::tolower);
}

std::string Encode::getOrder(char *buf, int &index, int maxsize) {
    // 获取字符串长度指令, key, value
    auto end = std::find_if(buf + index, buf + maxsize, [](char c) { return c == '\r' || c == ' ' || c == '\n'; });
    std::string order(buf + index, end);
    index += order.size() + 1;
    return order;
}

std::string Encode::getWord(std::string word) {    // 将字符串末尾加上换行
    word.append("\n");
    return word;
}

std::string Encode::sqlSplice(std::string sql, std::string word, int flag) {
    // 使用ostringstream可以更有效地拼接字符串，避免了频繁的内存分配和复制
    std::ostringstream oss;
    oss << sql << word.size() << " " << word;
    if (flag) {
        oss << " ";
    }
    return oss.str();
}
