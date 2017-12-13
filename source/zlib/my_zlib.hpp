#ifndef __MY_ZLIB_H
#define __MY_ZLIB_H
#include <zlib.h>
#include <string>
#include <cstring>

/*
*	zlib封装。主要解决解压后不能确定源文长度问题
*/
class myZlib{
public:
	myZlib() {

	}

	~myZlib() {

	}

	static int compress(const Byte * src, uLong srcLen, Byte * dest, uLong & destLen) {
		uLong tmpLen = compressBound(srcLen);
		std::string tmpStr(destLen + sizeof(uLong), 0);
		int ret = ::compress((Byte *)&tmpStr[0], &tmpLen, src, srcLen);
		if (Z_OK != ret) {
			return ret;
		}
	
		if (destLen < (tmpLen + sizeof(uLong))) {
			return Z_BUF_ERROR;
		}

		memcpy(dest, &srcLen, sizeof(uLong));
		memcpy(dest + sizeof(uLong), &tmpStr[0], tmpLen);
		destLen = sizeof(uLong) + tmpLen;

		return Z_OK;
	}

	static int compress(const std::string & src, std::string & dest) {
		uLong tmpLen = src.length();
		std::string tmp(tmpLen, 0);
		int ret = compress((const Byte *)src.c_str(), src.length(), (Byte*)&tmp[0], tmpLen);
		if (Z_OK != ret) {
			return ret;
		}

		tmp.resize(tmpLen);
		tmp.swap(dest);
		
		return Z_OK;
	}

	static int uncompress(const Byte * dest, uLong destLen, Byte * src, uLong & srcLen) {
		if ((NULL == dest) || destLen < 4) {
			return Z_DATA_ERROR;
		}

		uLong tmpLen = 0;
		memcpy(&tmpLen, dest, sizeof(uLong));
		if (NULL == src) {
			srcLen = tmpLen;
			return Z_OK;
		}

		if (srcLen < tmpLen) {
			return Z_BUF_ERROR;
		}

		return ::uncompress(src, &srcLen, dest + sizeof(uLong), destLen - sizeof(uLong));
	}

	static int uncompress(const std::string & dest, std::string & src) {
		uLong srcLen = 0;
		int ret = uncompress((const Byte*)dest.c_str(), dest.length(), NULL, srcLen);
		if (Z_OK != ret) {
			return ret;
		}

		std::string tmp(srcLen, 0);
		ret = uncompress((const Byte*)dest.c_str(), dest.length(), (Byte*)&tmp[0], srcLen);
		if (Z_OK != ret) {
	            return ret;
        	}
	
		tmp.resize(srcLen);
		tmp.swap(src);

		return Z_OK;
	}
};

#endif//__MY_ZLIB_H
