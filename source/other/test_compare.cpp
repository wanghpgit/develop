#include "compare.hpp"
#include <iostream>

int main () {
	int iVal1 = 100;
	int iVal2 = 999;

	double dVal1 = 100.00;
	double dVal2 = 999.99;

	std::cout << std::boolalpha;
	std::cout << iVal1 << " == " << iVal2 << "\t "<< wanghp::compare<int>::eq(iVal1, iVal2) << std::endl;
	std::cout << iVal1 << " != " << iVal2 << "\t "<< wanghp::compare<int>::ne(iVal1, iVal2) << std::endl;
	std::cout << iVal1 << " < " << iVal2 << "\t "<< wanghp::compare<int>::lt(iVal1, iVal2) << std::endl;
	std::cout << iVal1 << " <= " << iVal2 << "\t "<< wanghp::compare<int>::le(iVal1, iVal2) << std::endl;
	std::cout << iVal1 << " > " << iVal2 << "\t "<< wanghp::compare<int>::gt(iVal1, iVal2) << std::endl;
	std::cout << iVal1 << " >= " << iVal2 << "\t "<< wanghp::compare<int>::ge(iVal1, iVal2) << std::endl;
	
	return 0;
}
