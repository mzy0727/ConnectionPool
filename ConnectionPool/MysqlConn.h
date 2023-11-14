#pragma once
#include<iostream>
#include <mysql.h>
using namespace std;

class MysqlConn
{
	// ��ʼ�����ݿ�����
	MysqlConn();
	// �ͷ����ݿ�����
	~MysqlConn();
	// �������ݿ�
	bool connect(string user, string passwd, string dbName, string ip, unsigned short port = 3306);
	// �������ݿ⣺insert��update,delete
	bool update(string sql);
	// ��ѯ���ݿ�
	bool query(string sql);
	// ������ѯ�õ��Ľ����
	bool next();
	// �õ�������е��ֶ�ֵ
	string value(int index);
	// �������
	bool transaction();
	// �ύ����
	bool commit();
	// ����ع�
	bool rollback();
private:
	void freeResult();
	MYSQL* m_conn = nullptr;
	MYSQL_RES* m_result = nullptr;
	MYSQL_ROW m_row = nullptr;
};

