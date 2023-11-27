/*************************************************************************
	> File Name: Connection.cpp
	> Author: luochenhao
	> Mail: 3453851623@qq.com
	> Created Time: Fri 17 Nov 2023 05:03:06 PM CST
 ************************************************************************/

#include "Connection.h"

Connection::Connection()
{
	_conn = mysql_init(nullptr);
}

Connection::~Connection()
{
	if (_conn != nullptr) mysql_close(_conn);
}

bool Connection::connect(string ip, unsigned short port, string user, string password, string dbname)
{
	MYSQL *p = mysql_real_connect(_conn, ip.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0);
	return p != nullptr;
}

// insert delete update
bool Connection::update(string sql)
{
	if (mysql_query(_conn, sql.c_str())) {
		LOG("update failed!~" + sql);
		return false;
	}
	return mysql_use_result(_conn);
}

