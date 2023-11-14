#include "MysqlConn.h"

MysqlConn::MysqlConn()
{
	// 初始化连接环境
	m_conn = mysql_init(nullptr);
	// 为当前连接设置默认的字符集
	mysql_set_character_set(m_conn,"utf8" );
}

MysqlConn::~MysqlConn()
{
	// 释放资源
	if (m_conn != nullptr) {
		mysql_close(m_conn);
	}
	freeResult();
}

bool MysqlConn::connect(string user, string passwd, string dbName, string ip, unsigned short port)
{
	// 连接 mysql 的服务器
	MYSQL* ptr =  mysql_real_connect(m_conn, ip.c_str(),user.c_str(), passwd.c_str(), dbName.c_str(),port,nullptr,0);
	return ptr != nullptr;
}

bool MysqlConn::update(string sql)
{
	if (mysql_query(m_conn, sql.c_str())) {		//不管数据库中有没有要查询的用户名信息，只要select语句执行成功，mysql_query（）的返回值就都为0
		return false;
	}
	return true;
}

bool MysqlConn::query(string sql)
{
	freeResult();
	if (mysql_query(m_conn, sql.c_str())) {
		return false;
	}
	// 将查询的全部结果读取到客户端，分配1个MYSQL_RES结构
	m_result =  mysql_store_result(m_conn);
	return true;
}

bool MysqlConn::next()
{
	// 检索结果集的下一行
	if (m_result != nullptr) {
		
		m_row =mysql_fetch_row(m_result);
	}
	return false;
}

string MysqlConn::value(int index)
{
	// 返回结果集中的行数
	int rowCount = mysql_num_fields(m_result);
	if (index >= rowCount || index < 0) {
		return string();
	}
	char* val = m_row[index];
	// 返回结果集内当前行的列的长度
	unsigned long length =  mysql_fetch_lengths(m_result)[index];
	return string(val,length);
}

bool MysqlConn::transaction()
{
	// 如果模式为“1”，启用autocommit模式；如果模式为“0”，禁止autocommit模式
	return mysql_autocommit(m_conn,false);
}

bool MysqlConn::commit()
{
	// 提交当前事务
	return mysql_commit(m_conn);
}

bool MysqlConn::rollback()
{
	// 回滚当前事务
	return mysql_rollback(m_conn);
}

void MysqlConn::freeResult()
{
	if (m_result) {
		// 释放由mysql_store_result()、mysql_use_result()、mysql_list_dbs()等为结果集分配的内存。完成对结果集的操作后，必须调用mysql_free_result()释放结果集使用的内存。
		mysql_free_result(m_result);
		m_result = nullptr;
	}
}
