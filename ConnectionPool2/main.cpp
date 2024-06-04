#include <iostream>
#include <memory>
#include "MysqlConn.h"
#include "Sql_ConnectionPool.h"
#include <thread>
using namespace std;

 connection_pool *m_connPool = connection_pool::get_instance();


// 1. 单线程: 使用/不使用连接池
// 2. 多线程: 使用/不使用连接池

void op1(int begin, int end)
{
    for (int i = begin; i < end; ++i)
    {
        MysqlConn conn;
        conn.connect("root", "123456", "testdb2", "127.0.0.1");
        char sql[1024] = { 0 };
        sprintf(sql, "insert into preson values(%d, 25, 'man', 'tom')", i);
        conn.update(sql);
    }
}

void op2(MysqlConn* pool, int begin, int end)
{
    for (int i = begin; i < end; ++i)
    {
      //  shared_ptr<MysqlConn> conn = pool->getConnection();
        char sql[1024] = { 0 };
        sprintf(sql, "insert into preson values(%d, 25, 'man', 'tom')", i);
        pool->update(sql);
        cout << "op2 " << i << endl;
    }
    cout << "op2 finish" << endl;
}

void test1()
{
#if 0
    // 非连接池, 单线程, 用时: 21037278300 纳秒, 21037 毫秒
    steady_clock::time_point begin = steady_clock::now();
    op1(0, 10);
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    cout << "非连接池, 单线程, 用时: " << length.count() << " 纳秒, "
        << length.count() / 1000000 << " 毫秒" << endl;
#else
    // 连接池, 单线程, 用时: 8838406500 纳秒, 8838 毫秒
    MysqlConn *mysql = new MysqlConn;
    connectionRAII pollRAII(mysql, m_connPool);
    steady_clock::time_point begin = steady_clock::now();
    op2(mysql, 0, 10);
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    cout << "连接池, 单线程, 用时: " << length.count() << " 纳秒, "
        << length.count() / 1000000 << " 毫秒" << endl;

#endif
}

void test2()
{
#if 0
    // 非连接池, 多单线程, 用时: 13277417000 纳秒, 13277 毫秒
    MysqlConn conn;
    conn.connect("root", "root", "testdb2", "192.168.237.131");
    steady_clock::time_point begin = steady_clock::now();
    thread t1(op1, 0, 1000);
    thread t2(op1, 1000, 2000);
    thread t3(op1, 2000, 3000);
    thread t4(op1, 3000, 4000);
    thread t5(op1, 4000, 5000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    cout << "非连接池, 多单线程, 用时: " << length.count() << " 纳秒, "
        << length.count() / 1000000 << " 毫秒" << endl;

#else
    // 连接池, 多单线程, 用时: 3938502100 纳秒, 3938 毫秒
  //  connectionRAII* pollRAII ;
    MysqlConn *mysql = nullptr;
    connectionRAII pollRAII(mysql, m_connPool);
  //  ConnectionPool* pool = ConnectionPool::getConnectPool();
    steady_clock::time_point begin = steady_clock::now();
    thread t1(op2, mysql, 10000, 10010);
    // thread t2(op2, pool, 1000, 2000);
    // thread t3(op2, pool, 2000, 3000);
    // thread t4(op2, pool, 3000, 4000);
    // thread t5(op2, pool, 4000, 5000);
    t1.join();
    // t2.join();
    // t3.join();
    // t4.join();
    // t5.join();
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    cout << "连接池, 多单线程, 用时: " << length.count() << " 纳秒, "
        << length.count() / 1000000 << " 毫秒" << endl;

#endif
}

int query()
{
    MysqlConn conn;
    bool tt = conn.connect("root", "123456", "testdb2", "127.0.0.1");
    cout << "tt:  " << tt << endl;
    string sql = "insert into preson values(7, 25, 'man', 'tom')";
    bool flag = conn.update(sql);
    cout << "flag value:  " << flag << endl;

    sql = "select * from person";
    conn.query(sql);
    while (conn.next())
    {
        cout << conn.value(0) << ", "
            << conn.value(1) << ", "
            << conn.value(2) << ", "
            << conn.value(3) << endl;
    }
    return 0;
}

class A {
public:
    ~A() {
        cout << "finish" << endl;
    }
};

A a;

int main()
{
    m_connPool->init("127.0.0.1", "root", "123456", "testdb2", 3306, 10);
    
   // query();
   // test1();
    test1();
    return 0;
}
