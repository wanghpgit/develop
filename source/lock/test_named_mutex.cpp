#include "boost/interprocess/sync/named_mutex.hpp"
#include "boost/interprocess/sync/scoped_lock.hpp"
#include <iostream>

void test_named_mutex() {
	boost::interprocess::permissions per;
	per.set_unrestricted();
	boost::interprocess::named_mutex mutex(boost::interprocess::open_or_create, "haha", per);
	boost::interprocess::scoped_lock<boost::interprocess::named_mutex> lock(mutex);
	std::cout << "...." << std::endl;
	getchar();
}

int main() {
	test_named_mutex();
	return 0;
}
