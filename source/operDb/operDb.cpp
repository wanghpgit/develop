#include "operDb.h"
#include "operMysql.hpp"
#include <cstdarg>

/*边长参数格式化数据过长错误码*/

/*边长参数格式化宏适用于operDb类*/
#define FORMAT_VARIABLE_PARAMTER(str) {	va_list ap;\
										va_start(ap, format);\
										std::string tmp(MAX_SQL_LENGETH, 0);\
										int len = vsprintf(&tmp[0], format, ap);\
										if(MAX_SQL_LENGETH == len)\
										{return FORMAT_VARIABLE_PARAMTER_ERROR;}\
										tmp.resize(len);\
										str.swap(tmp);va_end(ap);}

#define CHECK_DB_PTR	if (NULL == m_db){return DB_PTR_NULL;}

const int operDb::MAX_SQL_LENGETH = 1024 * 4;
const int operDb::FORMAT_VARIABLE_PARAMTER_ERROR = 998;
const int operDb::DB_PTR_NULL = 997;

operDb::operDb(DB_TYPE type) {
	new_db(type, NULL);
}

operDb::operDb(DB_TYPE type, const std::string & para){
	new_db(type, para.c_str());
}

operDb::~operDb(){
	del_db();
}

int operDb::setParameter(const std::string & para){
	CHECK_DB_PTR;
	m_db->setParameter(para);
	return 0;
}

int operDb::selectDb(const std::string & sql, std::vector<std::vector<std::string> > & values){
	CHECK_DB_PTR;
	return m_db->selectDb(sql, values);
}

int operDb::selectDb(std::vector<std::vector<std::string> > & values, const char * format, ...){
	std::string sql;
	FORMAT_VARIABLE_PARAMTER(sql);
	return selectDb(sql, values);
}

int operDb::insertDb(const std::string & sql){
	CHECK_DB_PTR;
	return m_db->insertDb(sql);
}

int operDb::insertDb(const char * format, ...) {
	std::string sql;
	FORMAT_VARIABLE_PARAMTER(sql);
	return insertDb(sql);
}

int operDb::updateDb(const std::string & sql){
	CHECK_DB_PTR;
	return m_db->updateDb(sql);
}

int operDb::updateDb(const char * format, ...) {
	std::string sql;
	FORMAT_VARIABLE_PARAMTER(sql);
	return updateDb(sql);
}

int operDb::deleteDb(const std::string & sql){
	CHECK_DB_PTR;
	return m_db->deleteDb(sql);
}

int operDb::deleteDb(const char * format, ...) {
	std::string sql;
	FORMAT_VARIABLE_PARAMTER(sql);
	return deleteDb(sql);
}

void operDb::new_db(DB_TYPE type, const char * dbPara){
	switch (type) {
	case MYSQL_DB:
		m_db = (NULL ==dbPara) ? (new operMysql) : (new operMysql(dbPara));
		break;
	case UNKNOWN_DB:
	default:
		m_db = NULL;
		break;
	}
}

void operDb::del_db(){
	if (NULL != m_db) {
		delete m_db, m_db = NULL;
	}
}
