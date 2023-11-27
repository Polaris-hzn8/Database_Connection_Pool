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
    //对象自动析构
    Connection conn;
    char sql[1024] = {0};
    sprintf(sql, "insert into user values(name, age, sex) values('%s', '%d', '%s')", "zhangsan", 20, "male");
    conn.connect("39.99.133.193", 3306, "root", "20001201", "chat");
    conn.update(sql);
    return 0;
}
