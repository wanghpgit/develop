#include <iostream>
#include "my_zlib.hpp"

void test_zlib() {
	std::cout << "Z_OK:" << Z_OK << std::endl;
	std::cout << "Z_MEM_ERROR:" << Z_MEM_ERROR << std::endl;
	std::cout << "Z_BUF_ERROR:" << Z_BUF_ERROR << std::endl;
	std::cout << "Z_DATA_ERROR:" << Z_DATA_ERROR << std::endl;
	
	std::string strSrc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			     "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
			     "ccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
			     "ddddddddddddddddddddddddddddddddddddddddddddddddddddddd"
			     "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
			     "fffffffffffffffffffffffffffffffffffffffffffffffffffffff"
			     "ggggggggggggggggggggggggggggggggggggggggggggggggggggggg"
			     "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"
			     "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"
			     "jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj"
			     "kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk"			
			     "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			     "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
			     "ccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
			     "ddddddddddddddddddddddddddddddddddddddddddddddddddddddd"
			     "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
			     "fffffffffffffffffffffffffffffffffffffffffffffffffffffff"
			     "ggggggggggggggggggggggggggggggggggggggggggggggggggggggg"
			     "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"
			     "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"
			     "jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj"
			     "kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk"			
	;
	
	unsigned long strZlibLen = compressBound(strSrc.length()+1);;
	std::string strZlib(strSrc.length(), 0);
	
	int ret = compress((Bytef*)&strZlib[0], &strZlibLen, (const Bytef *)strSrc.c_str(), strSrc.length()+1);
	if (Z_OK != ret) {
		std::cout << "compress failed! ret:" << ret << std::endl;
		return ;
	}
	strZlib.resize(strZlibLen);

	std::cout << "strSrc len:" << strSrc.length() << std::endl;
	std::cout << "strSrc:" << strSrc << std::endl;
	std::cout << "strZlib len:" << strZlibLen << std::endl; 
	std::cout << "strZlib:" << strZlib << std::endl;

	unsigned long strTmpLen = strSrc.length()+1;
	std::cout << "strTmpLen: " << strTmpLen << std::endl;

	std::string strTmp(strTmpLen, 0);
	ret = uncompress((Bytef*)&strTmp[0], &strTmpLen, (const Bytef*)&strZlib[0], strZlibLen);
	if (Z_OK != ret) {
		std::cout << "uncompress failed! ret:" << ret << std::endl;
        	return ;
	}

	std::cout << "src len: " << strTmpLen << std::endl;
	std::cout << "src: " << strTmp << std::endl;
}

void test_myZlib() {
	std::string src = "111111111111wwwwwwwwwwwwwxxxxxxxxxxxxxxx4444444444444";
	uLong srcLen = src.length()+1;

	Byte dest[128] = {0};
	uLong destLen = sizeof(dest)/sizeof(Byte);
	
	int ret = myZlib::compress((const Byte*)src.c_str(), srcLen, dest, destLen);
	if (Z_OK != ret) {
		std::cout << "myZlib::compress failed! ret:" << ret << std::endl;
		return;
	}

	std::cout << "src len:" << src.length() + 1 << std::endl;
	std::cout << "src:" << src << std::endl;
	std::cout << "dest len:" << destLen << std::endl;
	std::cout << "dest:" << dest << std::endl;
	
	uLongf tmpLen = 0;
	ret = myZlib::uncompress(dest, destLen, NULL, tmpLen);
	if (Z_OK != ret) {
        std::cout << "myZlib::uncompress get len failed! ret:" << ret << std::endl;
        return;
    }
	
	std::cout << "tmp len:" << tmpLen << std::endl;
	
	std::string tmp(tmpLen, 0);
	ret = myZlib::uncompress(dest, destLen, (Byte*)&tmp[0], tmpLen);
    if (Z_OK != ret) {
        std::cout << "myZlib::uncompress failed! ret:" << ret << std::endl;
        return;
    }

	std::cout << "tmp:" << tmp << std::endl;
}

int main() {
	test_zlib();
	test_myZlib();
	return 0;
}
