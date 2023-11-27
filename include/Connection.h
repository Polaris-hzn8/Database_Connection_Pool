/*************************************************************************
	> File Name: Connection.h
	> Author: luochenhao
	> Mail: 3453851623@qq.com
	> Created Time: Fri 17 Nov 2023 05:02:51 PM CST
 ************************************************************************/

#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <ctime>
#include <string.h>
#include <iostream>
#include <mysql/mysql.h>
using namespace std;

#define LOG(str)									\
	cout << __FILE__ << ":" << __LINE__ << " " <<	\
	__TIMESTAMP__ << ":" << str << endl;

// 封装Connection对象 实现数据库增删改查
class Connection {
public:
	// 初始化数据库连接
	Connection();
    // 释放数据库连接资源
    ~Connection();
    // 连接数据库
    bool connect(string ip, unsigned short port, string user, string password, string dbname);
    // 更新数据库操作 insert delete update
    bool update(string sql);
    // 刷新连接的起始空闲时间点
    void refreshAliveTime() { _alivetime = clock(); }
    // 返回连接存活的时长
    clock_t getAliveTime() const { return clock() - _alivetime; }
private:
    // 表示和 MYSQL Server的一条连接
    MYSQL *_conn;
    // 进入空闲状态后的起始时间
    clock_t _alivetime;
};

#endif
