#include <limits> 
#include <iostream>
#include <sstream>
  
 void test_limits() {
	std::stringstream sFormat;
	sFormat << std::boolalpha;  
	// 可以表示的最大值  
	sFormat << "max(float): " << std::numeric_limits<float>::max() << std::endl;  
	// 可以表示的大于0的最小值，其他类型的实现或与此不同  
	sFormat << "min(float): " << std::numeric_limits<float>::min() << std::endl;  
	// 标准库是否为其实现了特化  
	sFormat << "is_specialized(float): " << std::numeric_limits<float>::is_specialized << std::endl;  
	// 是否是有符号的，即可以表示正负值  
	sFormat << "is_signed(float): " << std::numeric_limits<float>::is_signed << std::endl;  
	// 不否是整形的  
	sFormat << "is_integer(float): " << std::numeric_limits<float>::is_integer << std::endl;  
	// 是否是精确表示的  
	sFormat << "is_exact(float): " << std::numeric_limits<float>::is_exact << std::endl;  
	// 是否存在大小界限  
	sFormat << "is_bounded(float): " << std::numeric_limits<float>::is_bounded << std::endl;  
	// 两个比较大的数相加而不会溢出，生成一个较小的值  
	sFormat << "is_modulo(float): " << std::numeric_limits<float>::is_modulo << std::endl;  
	// 是否符合某某标准  
	sFormat << "is_iec559(float): " << std::numeric_limits<float>::is_iec559 << std::endl;  
	// 不加+-号可以表示的位数  
	sFormat << "digits(float): " << std::numeric_limits<float>::digits << std::endl;  
	// 十进制数的个数  
	sFormat << "digits10(float): " << std::numeric_limits<float>::digits10 << std::endl;  
	// 一般基数为2  
	sFormat << "radix(float): " << std::numeric_limits<float>::radix << std::endl;  
	// 以2为基数的最小指数  
	sFormat << "min_exponent(float): " << std::numeric_limits<float>::min_exponent << std::endl;  
	// 以2为基数的最大指数  
	sFormat << "max_exponent(float): " << std::numeric_limits<float>::max_exponent << std::endl;  
	// 以10为基数的最小指数  
	sFormat << "min_exponent10(float): " << std::numeric_limits<float>::min_exponent10 << std::endl;  
	// 以10为基数的最大指数  
	sFormat << "max_exponent10(float): " << std::numeric_limits<float>::max_exponent10 << std::endl;  
	// 1值和最接近1值的差距  
	sFormat << "epsilon(float): " << std::numeric_limits<float>::epsilon() << std::endl;  
	// 舍入方式  
	sFormat << "round_style(float): " << std::numeric_limits<float>::round_style << std::endl;  

	std::cout << sFormat.str() << std::cout.flush();
 } 
  
int main() {  
    test_limits();
	return 0;
}  
