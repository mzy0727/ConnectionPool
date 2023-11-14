#include "MysqlConn.h"

MysqlConn::MysqlConn()
{
	// ��ʼ�����ӻ���
	m_conn = mysql_init(nullptr);
	// Ϊ��ǰ��������Ĭ�ϵ��ַ���
	mysql_set_character_set(m_conn,"utf8" );
}

MysqlConn::~MysqlConn()
{
	// �ͷ���Դ
	if (m_conn != nullptr) {
		mysql_close(m_conn);
	}
	freeResult();
}

bool MysqlConn::connect(string user, string passwd, string dbName, string ip, unsigned short port)
{
	// ���� mysql �ķ�����
	MYSQL* ptr =  mysql_real_connect(m_conn, ip.c_str(),user.c_str(), passwd.c_str(), dbName.c_str(),port,nullptr,0);
	return ptr != nullptr;
}

bool MysqlConn::update(string sql)
{
	if (mysql_query(m_conn, sql.c_str())) {		//�������ݿ�����û��Ҫ��ѯ���û�����Ϣ��ֻҪselect���ִ�гɹ���mysql_query�����ķ���ֵ�Ͷ�Ϊ0
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
	// ����ѯ��ȫ�������ȡ���ͻ��ˣ�����1��MYSQL_RES�ṹ
	m_result =  mysql_store_result(m_conn);
	return true;
}

bool MysqlConn::next()
{
	// �������������һ��
	if (m_result != nullptr) {
		
		m_row =mysql_fetch_row(m_result);
	}
	return false;
}

string MysqlConn::value(int index)
{
	// ���ؽ�����е�����
	int rowCount = mysql_num_fields(m_result);
	if (index >= rowCount || index < 0) {
		return string();
	}
	char* val = m_row[index];
	// ���ؽ�����ڵ�ǰ�е��еĳ���
	unsigned long length =  mysql_fetch_lengths(m_result)[index];
	return string(val,length);
}

bool MysqlConn::transaction()
{
	// ���ģʽΪ��1��������autocommitģʽ�����ģʽΪ��0������ֹautocommitģʽ
	return mysql_autocommit(m_conn,false);
}

bool MysqlConn::commit()
{
	// �ύ��ǰ����
	return mysql_commit(m_conn);
}

bool MysqlConn::rollback()
{
	// �ع���ǰ����
	return mysql_rollback(m_conn);
}

void MysqlConn::freeResult()
{
	if (m_result) {
		// �ͷ���mysql_store_result()��mysql_use_result()��mysql_list_dbs()��Ϊ�����������ڴ档��ɶԽ�����Ĳ����󣬱������mysql_free_result()�ͷŽ����ʹ�õ��ڴ档
		mysql_free_result(m_result);
		m_result = nullptr;
	}
}
