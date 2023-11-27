/*************************************************************************
	> File Name: CommonConnectionPool.h
	> Author: luochenhao
	> Mail: 3453851623@qq.com
	> Created Time: Fri 17 Nov 2023 05:01:57 PM CST
 ************************************************************************/

#ifndef _COMMONCONNECTIONPOOL_H
#define _COMMONCONNECTIONPOOL_H

#include <queue>
#include <string>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <functional>
#include <condition_variable>
#include "Connection.h"
using namespace std;

// 实现连接池功能模块
class ConnectionPool {
public:
	// 获取连接池实例
	static ConnectionPool* getConnectionPool();
	// 获取数据库连接
	shared_ptr<Connection> getConnection();
private:
	ConnectionPool();
	// 加载外部配置文件
	bool loadConfigFile();
	// 生产新连接任务函数
	void produceConnTask();
	// 定时扫描空闲连接 并进行回收操作
	void collectConnTask();
private:
	// 连接池参数
	int _initSize;				// 初始连接量
	int _maxSize;				// 最大连接量
	int _maxIdleTime;			// 最大空闲时间
	int _connectionTimeout;		// 连接超时时间
	// 数据库信息
	string _ip;
	unsigned short _port;
	string _username;
	string _password;
	string _dbname;
	// 数据库连接存储队列
	queue<Connection*> _connectionq;		// 存储mysql连接的队列
	mutex _queueMutex;						// 维护连接队列的线程安全的互斥锁
	atomic_int _connectionCnt;				// 创建连接的数量 ++操作是线程安全的
	condition_variable condv;					// 条件变量 用于连接生产线程和连接消费线程间的通信
};

#endif
