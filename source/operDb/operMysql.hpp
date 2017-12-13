#ifndef __OPER_MYSQL_H
#define __OPER_MYSQL_H
#include "operDbBase.hpp"
#include "mysql.h"
#include <cstring>

#ifdef _DEBUG
#include <iostream>
#endif//_DEBUG

class operMysql : public operDbBase{
protected:
	friend class operDb;
	operMysql():operDbBase() {
		
	}

	operMysql(const std::string & para):operDbBase(para) {

	}
	
	virtual ~operMysql() {
		
	}

	virtual int selectDb(const std::string & sql, std::vector<std::vector<std::string> > & values)  {
		if (0 != connectDb()) {
			return 21;
		}

		MYSQL_ROW row;
		MYSQL_RES *res;
		std::vector<std::vector<std::string> > tmpValues;
		if(mysql_query(&m_mysql, sql.c_str()) != 0) {
			return 22;
		}

		res = mysql_store_result(&m_mysql);
		if(res==NULL) {
			return 23;
		}
#ifdef _DEBUG
		std::cout << __FUNCTION__ << "\t" << __LINE__ << std::endl;
#endif

		int count = mysql_num_fields(res);
#ifdef _DEBUG
		std::cout << __FUNCTION__ << "\t" << __LINE__ << std::endl;
		std::cout << "count:" << count << std::endl;
#endif

		while((row = mysql_fetch_row(res))) {
			std::vector<std::string> oneItem;
			unsigned long * lens = mysql_fetch_lengths(res);
			for(int i = 0; i<count; i++) {
				std::string tmp(lens[i], '\0');
				strncpy(&tmp[0], row[i], lens[i]);
#ifdef _DEBUG
				std::cout << tmp << "\t\t\t";
#endif
				oneItem.push_back(tmp);
			}
#ifdef _DEBUG
			std::cout << std::endl;
#endif
			tmpValues.push_back(oneItem);
		}

#ifdef _DEBUG
		std::cout << __FUNCTION__ << "\t" << __LINE__ << std::endl;
#endif

		mysql_free_result(res);
		tmpValues.swap(values);
		return 0;
	}

	virtual int insertDb(const std::string & sql) {
		return queryDb(sql);
	}

	virtual int updateDb(const std::string & sql) {
		return queryDb(sql);
	}

	virtual int deleteDb(const std::string & sql) {
		return queryDb(sql);
	}

protected:
	virtual int connectDb() {
		if (m_isConnect) {
			return 0;
		}
	
		std::string dbName;
		std::string user;
		std::string passwd; 
		std::string host = "localhost";
		std::string charset = "utf8";
		unsigned int port=3306;
		
#ifdef _DEBUG
		std::cout << "m_para: " << m_para << std::endl;
#endif	
			
		/*解析配置*/
		if (!getOneParameter("Server", host) ||	
			!getOneParameter("Database", dbName) ||	
			!getOneParameter("Uid", user) ||	
			!getOneParameter("Pwd", passwd) ||	
			!getOneParameter("CharSet", charset)) {
#ifdef _DEBUG
			std::cout << "getOneParameter failed!" << std::endl;
#endif				
			return 1;	
		}

#ifdef _DEBUG
		std::cout << "host:" << host << std::endl << "user:" << user << std::endl << "passwd:" << passwd << std::endl 
			  << "dbName:" << dbName << std::endl << "charset:" << charset << std::endl << "port:" << port << std::endl;
#endif
	
		if(NULL == mysql_init(&m_mysql))  {
			return 2;
		}
	
		if (NULL == mysql_real_connect(&m_mysql, host.c_str(), user.c_str(), passwd.c_str(), dbName.c_str(), port, NULL, 0)) {
			return 3;
		}
	
#ifdef _DEBUG
		std::cout << __FUNCTION__ << "\t" << __LINE__ << std::endl;
#endif
	
		if(0 != mysql_set_character_set(&m_mysql, charset.c_str())) {
			return 4;
		}
		m_isConnect = true;
	
#ifdef _DEBUG
		std::cout << __FUNCTION__ << "\t" << __LINE__ << std::endl;
#endif
		return 0;
	}

	virtual void closeDb() {
		if (m_isConnect)	{
			mysql_close(&m_mysql);
			m_isConnect = false;
		}
	}


	int queryDb(const std::string & sql){
		if (0 != connectDb()) {
			return 11;
		}

		if(0 != mysql_query(&m_mysql,sql.c_str())) {
			return 12;
		}
		return 0;
	}

	bool getOneParameter(const std::string & parName, std::string & parValue){
		typedef std::string::size_type str_pos;

		str_pos pos1 = m_para.find(parName);
		if (std::string::npos == pos1) {
			return false;
		}
		std::string tmp = m_para.substr(pos1);

		pos1 = tmp.find("=");
		str_pos pos2 = tmp.find(";");
		if (("CharSet" == parName) && (std::string::npos != pos1)) {
			parValue=tmp.substr(pos1+1);
		} else if (std::string::npos != pos1 || std::string::npos != pos2) {
			parValue=tmp.substr(pos1+1, pos2-pos1-1);
		} else {
			return false;
		}

		return true;
	}

private:

	MYSQL				m_mysql;
};

#endif//__OPER_MYSQL_H
