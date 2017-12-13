#include "boost/any.hpp"
#include <vector>
#include <iostream>

void test_any(){
	std::vector<boost::any> vec;
	vec.push_back(123);
	vec.push_back(std::string("xxxasdasfa"));
	vec.push_back(1.2345);
	vec.push_back('c');

	for (std::vector<boost::any>::const_iterator cit=vec.begin(); cit != vec.end(); ++cit) {
		if (typeid(int) == cit->type()){
			std::cout << boost::any_cast<int>(*cit) << std::endl;
		}else if (typeid(std::string) == cit->type()) {
			std::cout << boost::any_cast<std::string>(*cit) << std::endl;
		}else if (typeid(double) == cit->type()){
			std::cout << boost::any_cast<double>(*cit) << std::endl;
		}else if (typeid(char) == cit->type()){
			std::cout << boost::any_cast<char>(*cit) << std::endl;
		}else{
			std::cout << "unknown type" << std::endl;
		}
	}
}

int main() {
	test_any();
	return 0;
}
