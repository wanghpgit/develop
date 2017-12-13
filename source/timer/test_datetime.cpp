#include "my_time.hpp"
#include <iostream>

void test_datetime() {
/*	struct tm _tm;
	std::string datetime = "2017-10-25 01:49:12";
	strptime(datetime.c_str(), "%Y-%m-%d %H:%M:%S", &_tm);
	time_t t = mktime(&_tm);
	std::cout << "t:" << t << std::endl;

	char buff[128] = {0};
	_tm = *localtime(&t);
	strftime(buff, 128, "%Y-%m-%d %H:%M:%S", &_tm);
	std::cout << "buff:" << buff << std::endl;
*/

	std::string datetime = "2017-10-25 01:49:12";
	std::string format = "%Y-%m-%d %H:%M:%S";
	time_t t = 0;
	if (0 != my_time::datetime_2_time(datetime, format, t)) {
		std::cout << "my_time::datetime_2_time failed!" << std::endl;
		return;
	}
	std::cout << "t:" << t << "###" << std::endl;

	std::string tmp;
	if (0 != my_time::time_2datetime(t, format, tmp)) {

	}
	std::cout << "tmp:" << tmp << "$$$" << std::endl;
	
	
}

int main() {
	test_datetime();
	return 0;
}
