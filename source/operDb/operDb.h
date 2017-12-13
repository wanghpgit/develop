#ifndef __OPER_DB_H
#define __OPER_DB_H
#include <string>
#include <vector>

class operDbBase;

class operDb{
public:
	static const int	MAX_SQL_LENGETH;
	static const int	FORMAT_VARIABLE_PARAMTER_ERROR;
	static const int	DB_PTR_NULL;

	typedef enum db_type{
		UNKNOWN_DB,
		MYSQL_DB
	} DB_TYPE;

public:
	operDb(DB_TYPE type);

	operDb(DB_TYPE type, const std::string & para);

	~operDb();

	int setParameter(const std::string & para);

	int selectDb(const std::string & sql, std::vector<std::vector<std::string> > & values);

	int selectDb(std::vector<std::vector<std::string> > & values, const char * format, ...);
	
	int insertDb(const std::string & sql);

	int insertDb(const char * format, ...);

	int updateDb(const std::string & sql);

	int updateDb(const char * format, ...) ;

	int deleteDb(const std::string & sql);

	int deleteDb(const char * format, ...);

private:
	void new_db(DB_TYPE type, const char * dbPara);
	void del_db();

private:
	operDbBase *		m_db;
};
#endif//__OPER_DB_H
