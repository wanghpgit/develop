#include <iostream>
#include <new>
#include <string>
#include <stdexcept>
#include <cstring>
#include <cstdio>
#include "boost/smart_ptr/scoped_array.hpp"
#include "boost/format.hpp"

#ifndef byte
typedef unsigned char byte;
#endif

class byte_arr{
public:
	typedef enum format_type {
		format_char,
		format_int_16,
		format_int_10,
		format_int_8
	}format_type; 

	byte_arr(): m_length(0), m_buff_size(0){}
	byte_arr(const byte * ptr, size_t len){
		this->reset(ptr, len);
	} 
	byte_arr(const byte_arr & that){
		this->reset(that.get(), that.length());
	}
	~byte_arr(){}

	byte_arr operator= (const byte_arr & that){
		if (this != &that)	{
			this->reset(that.m_ptr.get(), that.m_length);
		}
		return * this;
	}

	void reset(const byte * ptr, size_t len) {
//		try {
			byte * p = new byte[len];
			m_ptr.reset(p);
			memcpy(m_ptr.get(), ptr, len);
// 		} catch (std::bad_alloc & e) {
// 			std::cout << "void reset(const byte * ptr, size_t len) " << e.what() << std::endl;
// 			exit(1);
// 		}
		m_buff_size = m_length = len;
	}

	void change_buff(const byte * ptr, size_t len){
		if (m_buff_size<len){
			throw std::overflow_error("void change_buff(const byte * ptr, size_t len)");
		}

		memcpy(m_ptr.get(), ptr, len);
		if (m_length>len){
			memset(m_ptr.get()+len, 0, m_length - len); 
		}
		m_length = len;
	}

	byte * get() const{
		return m_ptr.get();
	}
	
	size_t length() const {
		return m_length;
	}

	size_t buff_size() const {
		return m_buff_size;
	}

	bool operator == (const byte_arr & that) const {
		if ((this->m_length == that.m_length) &&
			(0 == memcmp(this->m_ptr.get(), that.m_ptr.get(), that.length())))
		{
			return true;
		} else {
			return false;
		}		
	}

	bool operator != (const byte_arr & that) const {
		return !operator==(that);
	}

	std::string format(format_type type = format_int_16) const {
		boost::format fmt_size("buff_size:%d length:%d\t");
		//boost::format fmt_buff("0x%02x,");
		boost::format fmt_buff;
		switch (type) {
		case format_char:
			fmt_buff.parse("%c,");
			break;
		case format_int_16:
			fmt_buff.parse("0x%02x,");
			break;
		case format_int_10:
			fmt_buff.parse("%3d,");
			break;
		case format_int_8:
			fmt_buff.parse("%04o,");
			break;
		}
		fmt_size%m_buff_size%m_length;
		std::string tmp(fmt_size.str());
		for (int i=0; i!=m_length; ++i){
			if ((type == format_char)) {
				tmp += (fmt_buff%m_ptr.get()[i]).str();
			} else {
				tmp += (fmt_buff%static_cast<short>(m_ptr.get()[i])).str();
			}
			fmt_buff.clear();
		}
		
		tmp[tmp.length()] = '\0';
		return tmp;		
	}

private: 
	size_t						m_length;
	size_t						m_buff_size;
	boost::scoped_array<byte>	m_ptr;
};

std::ostream & operator << (std::ostream & os, const byte_arr & that){
	os << that.format(byte_arr::format_char) << std::endl;
	os << that.format(byte_arr::format_int_16) << std::endl;
	os << that.format(byte_arr::format_int_10) << std::endl;
	os << that.format(byte_arr::format_int_8) << std::endl;
	return os;
}

class A {
public:
	A():m_ptr(NULL){}
	A(const char * ptr) try : m_ptr(new char[strlen(ptr)+1]){
		strncpy(m_ptr, ptr, strlen(ptr));
		m_ptr[strlen(m_ptr)] = '0';
	} catch (const std::bad_alloc & e){
		std::cout << "(char * ptr) :" << e.what() << std::endl;
		exit(1);
	}

	~A() {
		if (m_ptr){
			delete[] m_ptr, m_ptr = NULL;
		}
	}
private:
	char * m_ptr;
};


int main() {
	std::cout << "char:" << sizeof(char) << std::endl;
	std::cout << "int:" << sizeof(int) << std::endl;
	std::cout << "short:" << sizeof(short) << std::endl;

	byte arr1[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

	byte_arr ba1(arr1, sizeof(arr1));
	for (int i=0; i<sizeof(arr1); ++i) {
		arr1[i] -= 32;
	}	

	byte_arr ba2;
	ba2.reset(arr1, sizeof(arr1));
	byte * p = ba2.get();
	size_t len = ba2.length();
	const byte_arr ba3 = ba2;
	
	std::cout << "ba1: "<< ba1 << std::endl;
	std::cout << "ba2: "<< ba2 << std::endl;
	std::cout << "ba3: "<< ba3 << std::endl;
	std::cout << "ba1 == ba2 : " << (ba1 == ba2) << std::endl;
	std::cout << "ba2 != ba3 : " << (ba2 != ba3) << std::endl;

	
	return 0;
}