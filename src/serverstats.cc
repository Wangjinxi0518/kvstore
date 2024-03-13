/**
 * @File Name: serverstats.cc
 * @brief : 这段代码定义了一个名为ServerStats的类，该类用于收集和提供服务器的统计信息(性能检测)。
// 统计信息:
// 服务器启动时间
// 已处理的命令总数
// 输入和输出的字节数
// 接收的连接总数
// 当前连接的客户端数量
// 每秒查询（QPS）
// 输入和输出的KB/s
 * @Author : wangjinxi email:1023026823@qq.com
 *
 */
#include "serverstats.h"
#include "kv_conn.h"
#include <ctime>
#include <iostream>
#include <map>
#include <assert.h>
#include <glog/logging.h>
#include <time.h>

ServerStats *ServerStats::serverstats_ = nullptr;

ServerStats::ServerStats()
    : old_time_(0),
      old_total_command_processed_(0),
      current_total_command_processed_(0),
      old_total_input_bytes_(0),
      current_total_input_bytes_(0),
      old_total_output_bytes_(0),
      current_total_output_bytes_(0),
      total_connections_received_(0),
      server_begin_time_(time(NULL)),
      qps_(0) {
}

ServerStats::~ServerStats() {
}

void ServerStats::Init() {
    if (!serverstats_) {
        serverstats_ = new ServerStats();
    }
}

ServerStats *ServerStats::GetCurrent() {
    assert(serverstats_ != nullptr);
    return serverstats_;
}

int ServerStats::GetServerBeginTime() {
    return server_begin_time_;
}

void ServerStats::Calculate(int size) {
    connect_client_ = size;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    current_time_ = (int)ltm->tm_sec;
    if (current_time_ != old_time_) {
        qps_ = current_total_command_processed_ - old_total_command_processed_;
        input_kbps_ = (current_total_input_bytes_ - old_total_input_bytes_) / 1048576;
        output_kbps_ = (current_total_output_bytes_ - old_total_output_bytes_) / 1024;
        old_total_command_processed_ = current_total_command_processed_;
        old_total_input_bytes_ = current_total_input_bytes_;
        old_total_output_bytes_ = current_total_output_bytes_;
        old_time_ = current_time_;
    }
}

int ServerStats::GetQps() {
    return qps_;
}

int ServerStats::GetInputKbps() {
    return input_kbps_;
}

int ServerStats::GetOutputKbps() {
    return output_kbps_;
}

int ServerStats::GetInputBytes() {
    return current_total_input_bytes_;
}

int ServerStats::GetOutputBytes() {
    return current_total_output_bytes_;
}

int ServerStats::GetTotalConnectionsReceived() {
    return total_connections_received_;
}

int ServerStats::GetConnectClient() {
    return connect_client_;
}

int ServerStats::GetTotalCommandsProcessed() {
    return current_total_command_processed_;
}

void ServerStats::AddCommandProcessed() {
    current_total_command_processed_++;
}

void ServerStats::AddInputBytes(int size) {
    current_total_input_bytes_ += size;
}

void ServerStats::AddOutputBytes(int size) {
    current_total_output_bytes_ += size;
}

void ServerStats::AddTotalConnectionsReceived() {
    total_connections_received_++;
}
