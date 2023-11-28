/*************************************************************************
    > File Name: main.cpp
    > Author: luochenhao
    > Mail: 3453851623@qq.com
    > Created Time: Fri 17 Nov 2023 17:00:15 CST
************************************************************************/

#include <iostream>
#include "Connection.h"
using namespace std;

int main() {
    cout << "start test..." << endl;
    Connection conn;//对象自动析构
    // 构造sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into user(id, name, password, state) values('%d', '%s', '%s', '%s')", 1000, "zhangsan", "123456", "offline");
    int port = 3306;
    string ip = "127.0.0.1";
    string username = "root";
    string password = "20001201";
    string dbname = "chat";
    // 执行连接查询操作
    bool res = conn.connect(ip, port, username, password, dbname);
    if (res) {
        cout << "mysql connect success!~" << endl;
        // insert into user(id, name, password, state) values('1000', 'zhangsan', '123456', 'offline');
        conn.update("SET CHARACTER SET GBK");
        conn.update("select * from user;");
        conn.update("insert into user(id, name, password, state) values(1002, 'lisi', '123456', 'offline');");
    } else {
        cout << "mysql connect failed!~" << endl;
    }
    cout << "end test." << endl;
    return 0;
}
