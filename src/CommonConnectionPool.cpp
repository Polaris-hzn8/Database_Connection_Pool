/*************************************************************************
	> File Name: CommonConnectionPool.cpp
	> Author: luochenhao
	> Mail: 3453851623@qq.com
	> Created Time: Fri 17 Nov 2023 04:59:48 PM CST
 ************************************************************************/

#include "public.h"
#include "CommonConnectionPool.h"

// 懒汉单例模式
// 对于静态局部变量的初始化 由编译器自动进行 lock与unlock
ConnectionPool* ConnectionPool::getConnectionPool()
{
	static ConnectionPool pool;
    return &pool;
}

// 初始化连接池
ConnectionPool::ConnectionPool() {
	// 1.加载配置项
	if (!loadConfigFile()) return;
	// 2.创建初始数量的连接
	for (int i = 0; i < _initSize; ++i) {
		Connection *p = new Connection();
		p->connect(_ip, _port, _username, _password, _dbname);
		_connectionq.push(p);
		_connectionCnt++;
	}
	// 3.启动一个新的线程作为连接的生产者
}

// 读取配置文件
bool ConnectionPool::loadConfigFile()
{
	FILE *fp = fopen("config.conf", "r");
	if (fp == nullptr) {
		LOG("Config file error, file not exists!");
		return false;
	}
	while (!feof(fp)) {
		// 逐行读取配置文件
		char line[1024] = { 0 };
		fgets(line, 1024, fp);
		string str = line;
		// 查找配置项
		int idx = str.find('=', 0);
		if (idx == -1) continue;// 无效配置项
		int edx = str.find('\n', 0);
		string key = str.substr(0, idx);
		string value = str.substr(idx + 1, edx - idx - 1);
		// 初始化成员变量
		if (key == "ip") {
			_ip = value;
		} else if (key == "port") {
			_port = atoi(value.c_str());
		} else if (key == "username") {
			_username = value;
		} else if (key == "password") {
			_password = value;
		} else if (key == "dbname") {
			_dbname = value;
		} else if (key == "initSize") {
			_initSize = atoi(value.c_str());
		} else if (key == "maxSize") {
			_maxSize = atoi(value.c_str());
		} else if (key == "maxIdleTime") {
			_maxIdleTime = atoi(value.c_str());
		} else if (key == "connectionTimeout") {
			_connectionTimeout = atoi(value.c_str());
		}
	}
	return true;
}


