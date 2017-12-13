#include "operDb.h"
#include <iostream>

void test_operMysql() {
	std::string dbPara = "Driver=libmyodbc5.so; Server=localhost; Database=UAP; Uid=root; Pwd=root; DSN=myodbc5; CharSet=UTF8";

	 operDb operdb(operDb::MYSQL_DB ,dbPara); 

	std::string insterSql = "INSERT INTO SYSTEM_CONFIG VALUES ('uap_serviceIDxxx', 'servidid', 'UAP业务系统ID', '')";

	std::cout << "\n================================================================================================\n" 
			  << "插入操作！sql:" <<  insterSql << std::endl;
	int ret = operdb.insertDb(insterSql);
	if (0 != ret) {
		std::cout << "operdb.insertDb failed! ret = " << ret << std::endl;
		return;
	}
	std::cout << "operdb.insertDb ok!" << std::endl;

	std::string updateSql = "UPDATE SYSTEM_CONFIG SET CFG_VALUE = '1234001@ssdfsfsdf'  where CFG_NAME ='uap_serviceIDxxx'";
	std::cout << "\n================================================================================================\n" 
			  << "更新操作！sql:" << updateSql << std::endl;
	ret = operdb.updateDb(updateSql);
	if (0 != ret) {
		std::cout << "operdb.updateDb failed! ret = " << ret << std::endl;
		return;
	}
	std::cout << "operdb.updateDb ok!" << std::endl;
	
	std::cout << "\n================================================================================================\n" 
			  << "查询单条数据" << std::endl;
	std::vector<std::vector<std::string> > values;
	ret = operdb.selectDb(values, "SELECT CFG_NAME, CFG_VALUE, CFG_DESC from SYSTEM_CONFIG WHERE CFG_NAME = '%s'", "uap_serviceIDxxx");
	if (0 != ret) {
		std::cout << "operdb.selectDb failed! ret = " << ret << std::endl;
		return;
	}
	std::cout << "operdb.selectDb ok!" << std::endl;
	std::cout << "CFG_NAME: "<< values[0][0] << std::endl;
	std::cout << "CFG_VALUE: "<< values[0][1] << std::endl;
	std::cout << "CFG_DESC: "<< values[0][2] << std::endl;
	
	std::cout << "\n================================================================================================\n" 
			  << "删除数据！" << std::endl;
	ret=operdb.deleteDb("DELETE FROM SYSTEM_CONFIG WHERE CFG_NAME = '%s'", "uap_serviceIDxxx");
	if (0 != ret) {
		std::cout << "operdb.deleteDb failed! ret = " << ret << std::endl;
		return;
	}
	std::cout << "operdb.deleteDb ok!" << std::endl;

	std::string selectSql = "SELECT CFG_NAME, CFG_VALUE, CFG_DESC from SYSTEM_CONFIG";
	std::cout << "\n================================================================================================\n" 
				  << "查询多条多字段数据" << std::endl;
	values.clear();
	ret = operdb.selectDb(values, selectSql.c_str());
	if (0 != ret) {
		std::cout << "operdb.selectDb failed! ret = " << ret << std::endl;
		return;
	}
	std::cout << "operdb.selectDb ok!" << std::endl;

	for (int i=0;i<values.size(); ++i) {
		for (int j=0; j<3; ++j) {
			std::cout << values[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}


int main () {
	test_operMysql();
	return 0;
}
