#include "boost/pool/pool.hpp"
#include <iostream>

void test_pool() {
	boost::pool<> mp_char(sizeof(char));
	size_t buff_len = 100;
	char * p_buff = reinterpret_cast<char*>(mp_char.ordered_malloc(buff_len));
	std::cout << "p_buff:" << &p_buff << std::endl;
	for (int i=0; i<buff_len; ++i) {
		std::cout << std::hex << (unsigned short)p_buff[i] << " ";
	}
	std::cout << std::endl;
}

int main() {
//	std::cout << "short size:" << sizeof(short) << std::endl;
//	std::cout << "int size:" << sizeof(int) << std::endl;
//	std::cout << "char size:" << sizeof(char) << std::endl; 
	test_pool();
	return 0;
}
