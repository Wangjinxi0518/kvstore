#include <iostream>
#include <fstream>
#include <vector>
#include "Encode.h"
#define CFG_FILE "./config"
using namespace std;

class IO_server {
  public:  // 公共部分函数   
      
  void set(string order, string key, string value) {  // set函数参数指令,key,value
    int maxpos;
    ofstream ofs;
    string sql = "";
    ofs.open("test.txt", ios::app); // 供写，文件不存在则创建，若文件已存在则在原文件内容后写入新的内容，指针位置总在最后
    maxpos = ofs.tellp();
    if (maxpos == 0) {
     ofs << "begin: " << endl;
    }
    EncodeFix a;
    sql = a.sqlSplice(sql, order, 1);
    sql = a.sqlSplice(sql, key, 1);
    sql = a.sqlSplice(sql, value, 0);
    ofs << sql << endl;  // 写入文件
    ofs.close(); // 关闭文件
  }

  string get(string Key) { // get函数参数指令，key
    int maxpos; // 文本的字符数量
    char x; // 文件每次读入的字符
    bool flag = false;
    EncodeFix a;
    ifstream f("test.txt", ios::ate); // 文件打开时，指针在文件最后
    string order, key, value, line = "";
    if (!f) {
       cout << "打开文件失败" << endl;
    }
    maxpos = f.tellg(); // 计算整个文本的字符数量
    f.seekg(ios::end); // 将指针放置于文件末尾
    for (int index = 2; index <= maxpos; index++) { 
      f.seekg(-index, ios::end); // 从文件末尾最后一个字符开始读
      f.get(x); // 每次获取一个字符
      if (x == 0) { // 如果当前没有字符则跳过
         continue;
      }
      if (x == '\n') { // 如果遇到换行说明此时一行的语句已经读完了
        string templine = line; // 将读到的一行存入templine
        if (templine.size() > 0) { // 判断这行语句是空语句还是非空
          position = 0; // 初始化位置
          order = Extract(templine);
          cout << "------------------------------" << endl;
          cout << "order: " << order << endl;
          key = Extract(templine);
          cout << "key: " << key << endl;
          value = Extract(templine);
          cout << "value: " << value << endl;
          cout << "------------------------------" << endl;
          if (key.compare(Key) == 0 && !value.empty()) { // 判断文件是否有对应的key以及此时的key有没有value值
            flag = true;
            value = a.getWord(value);
            break;
          }
        }
        line = "";
      } else {
        line = x + line; // 添加每个字符到line
      }
      x = 0; // 初始化
    }
    if (flag) {
      return value;  // 找到返回value
    } else {
      return a.getWord("not found"); // 未找到返回not found
    }
  }

  string flushall() {  // 使用文件清空
      std::fstream f("test.txt", std::fstream::out | std::ios_base::trunc); // 清空文件
    return "clear\n";
  }
  
  void Flushall() { // 清空leveldb数据库
    int flag = system( "rm -rf /root/Git/KV-Server/leveldb.db");
  }

  string Extract(string templine) { // 提取字符串中的key，value
    string word = ""; // 用来存字符串大小例如3 set 3 123 3 456中set前面的3这样的数值
    string str = ""; // 返回值
    int size;
    for (int i = position; ; i++) {
      if (templine[i] != ' ') {
        word.push_back(templine[i]); // 存入数值大小的字符串
      } else {
        position = i + 1; // 标记当前位置
        size = atoi(word.c_str());  // 将字符串转为int表示大小
        str.assign(templine, position, size); //在templine中 从position开始最多size个字符往后读
        position = position + size + 1; //变更position位置使其跳转到接下来一个参数的对应数值位的字符串开始
        break;
      }
    }
    return str;
  }

  private:
    int position;
};