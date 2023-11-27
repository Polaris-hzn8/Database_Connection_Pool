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

// 生产者线程的线程函数
void ConnectionPool::produceConnTask()
{
	for (;;) {
		// 队列上锁
		unique_lock<mutex> lock(_queueMutex);
		// 若队列不空 生产者线程进入等待状态 并且将锁释放 等待为空
		while (!_connectionq.empty()) {
			condv.wait(lock);
		}
		// 连接数量没有到达上限 则继续创建新的连接
		if (_connectionCnt < _maxSize) {
			Connection *p = new Connection();
			p->connect(_ip, _port, _username, _password, _dbname);
			p->refreshAliveTime();
			_connectionq.push(p);
			_connectionCnt++;
		}
		// 通知消费者线程可以消费连接了
		condv.notify_all();
	}
}

// 获取数据库连接
shared_ptr<Connection> ConnectionPool::getConnection()
{
	// 队列上锁
	unique_lock<mutex> lock(_queueMutex);
	// 如果队列为空 则将锁释放 等待最大超时时间
	if (_connectionq.empty()) {
		// 在指定的时间之内 如果有可获取的连接 则直接将连接取走
		condv.wait_for(lock, chrono::milliseconds(_connectionTimeout));
		// 线程被唤醒之后队列仍然为空 则获取连接失败
		if (_connectionq.empty()) {
			LOG("获取空闲连接超时...");
			return nullptr;
		}
	}
	// 返回连接对象 使用智能指针管理
	shared_ptr<Connection> sp(_connectionq.front(), [&](Connection *pcon){
		unique_lock<mutex> lock(_queueMutex);
		pcon->refreshAliveTime();
		_connectionq.push(pcon);
	});
	_connectionq.pop();
	// 通知生产者线程检查队列是否为空
	condv.notify_all();
    return sp;
}

// 初始化连接池
ConnectionPool::ConnectionPool() {
	// 1.加载配置项
	if (!loadConfigFile()) return;
	// 2.创建初始数量的连接
	for (int i = 0; i < _initSize; ++i) {
		Connection *p = new Connection();
		p->connect(_ip, _port, _username, _password, _dbname);
		p->refreshAliveTime();
		_connectionq.push(p);
		_connectionCnt++;
	}
	// 3.启动一个新的线程作为连接的生产者
	thread connProducer(std::bind(&ConnectionPool::produceConnTask, this));
	// 设置为守护线程
	connProducer.detach();
	// 4.启动一个新的线程 定时扫描超过maxIdleTime时间的空闲连接 对空闲连接进行回收操作
	thread connCollector(std::bind(&ConnectionPool::collectConnTask, this));
	// 设置为守护线程
	connCollector.detach();
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

// 定时扫描空闲连接 并进行回收操作
void ConnectionPool::collectConnTask()
{
	for (;;) {
		// 通过调用sleep模拟定时效果
		this_thread::sleep_for(chrono::seconds(_maxIdleTime));
		// 扫描整个连接池队列 释放多余连接
		unique_lock<mutex> lock(_queueMutex);
		while (_connectionCnt > _initSize) {
			Connection *p = _connectionq.front();
			if (p->getAliveTime() < (_maxIdleTime * 1000)) break;
			_connectionq.pop();
			_connectionCnt--;
			delete p;//调用Connection的析构函数释放连接
		}
	}
}

