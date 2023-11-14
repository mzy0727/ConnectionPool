#pragma once
#include<iostream>
#include <mysql.h>
using namespace std;

class MysqlConn
{
	// 初始化数据库连接
	MysqlConn();
	// 释放数据库连接
	~MysqlConn();
	// 连接数据库
	bool connect(string user, string passwd, string dbName, string ip, unsigned short port = 3306);
	// 更新数据库：insert，update,delete
	bool update(string sql);
	// 查询数据库
	bool query(string sql);
	// 遍历查询得到的结果集
	bool next();
	// 得到结果集中的字段值
	string value(int index);
	// 事物操作
	bool transaction();
	// 提交事物
	bool commit();
	// 事物回滚
	bool rollback();
private:
	void freeResult();
	MYSQL* m_conn = nullptr;
	MYSQL_RES* m_result = nullptr;
	MYSQL_ROW m_row = nullptr;
};

