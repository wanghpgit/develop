#include <ctime>
#include <string>

class my_time{
public:
	my_time() {
	}

	~my_time() {
	}

	static int datetime_2_time(const std::string & datetime, const std::string & format, time_t & t) {
		struct tm _tm;
#ifdef _WIN32
		//sscanf
		return 1;
#else
		if (NULL ==  strptime(datetime.c_str(), format.c_str(), &_tm)) {
			return 1;
		}
#endif
		t = mktime(&_tm);
		return 0;
	}

	static int time_2datetime(time_t t, const std::string & format, std::string & datetime){
		struct tm * _tm = localtime(&t);
		if (NULL == _tm) {
			return 1;
		}
		std::string tmp(128, 0);
#ifdef _WIN32
		return 2;
#else
		size_t len = strftime(&tmp[0], tmp.length(), format.c_str(), _tm);
		tmp.resize(len);
#endif
		tmp.swap(datetime);
		return 0;
	}
};
