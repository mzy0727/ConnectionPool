#ifndef SQL_CONNECTION_POOL_H
#define SQL_CONNECTION_POOL_H

#include <cstdio>
#include <list>
#include <mysql/mysql.h>
#include <error.h>
#include <cstring>
#include <iostream>
#include <string>
#include <mutex>
#include <semaphore.h>
#include "MysqlConn.h"

using namespace std;

// 数据库连接池
class connection_pool{
public:
    // 单例模式
    static connection_pool* get_instance(){
        static connection_pool connPool;
        return &connPool;
    }

    void init(string url,string user, string password, string database_name, unsigned short port, int max_conn);
    MysqlConn* get_connection();
    bool release_connection(MysqlConn *conn);
    int get_freeconn();
private:
    connection_pool();
    ~connection_pool();
    // 防止复制构造和赋值操作，确保单例的唯一性
    connection_pool(const connection_pool&) = delete;
    connection_pool& operator=(const connection_pool&) = delete;

    string m_url;   // 主机地址
    unsigned short m_port;  // 数据库端口号
    string m_user;  // 数据库用户名
    string m_password;  // 数据库密码
    string m_database_name; // 数据库名
    string m_close_log; // 日志开关

    int m_max_conn; // 最大连接数
    int m_cur_conn; // 当前已使用连接数
    int m_free_conn;    // 当前空闲连接数
    mutex m_mutexQ;   // 互斥锁
    list<MysqlConn*> conn_list; // 连接池
    sem_t reserve;  // 信号量，指示是否有空闲连接


};

// 资源获取即初始化

class connectionRAII{
public:
    connectionRAII(MysqlConn *conn, connection_pool *connPool);
    ~connectionRAII();
private:
    MysqlConn *connRAII;
    connection_pool *poolRAII;
};

#endif