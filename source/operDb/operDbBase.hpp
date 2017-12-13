#ifndef __OPER_DB_BASE_H
#define __OPER_DB_BASE_H
#include <string>
#include <cstdarg>
#include <cstdio>
#include <vector>

/*边长参数格式化数据过长错误码*/
const int FORMAT_VARIABLE_PARAMTER_ERROR = 998;

/*边长参数格式化宏适用于operDb类*/
#define FORMAT_VARIABLE_PARAMTER(str) {	va_list ap;\
										va_start(ap, format);\
										std::string tmp(MAX_SQL_LENGETH, 0);\
										int len = vsprintf(&tmp[0], format, ap);\
										if(MAX_SQL_LENGETH == len)\
										{return FORMAT_VARIABLE_PARAMTER_ERROR;}\
										tmp.resize(len);\
										str.swap(tmp);va_end(ap);}
class operDb;

class operDbBase{
protected:
	friend class operDb;
	operDbBase() : m_isConnect(false){

	}

	operDbBase(const std::string & para) : m_isConnect(false){
		setParameter(para);
	}

	virtual ~operDbBase(){
		closeDb();
	}
	
	virtual void setParameter(const std::string & para){
		m_para = para;
	}

	virtual int selectDb(const std::string & sql, std::vector<std::vector<std::string> > & values) = 0;

	virtual int insertDb(const std::string & sql) = 0;

	virtual int updateDb(const std::string & sql) = 0;

	virtual int deleteDb(const std::string & sql) = 0;

	virtual int connectDb() = 0;

	virtual void closeDb(){

	}

protected:
	bool				m_isConnect;
	std::string			m_para;
};

#endif//__OPER_DB_BASE_H
