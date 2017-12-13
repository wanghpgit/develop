#include <iostream>

void test() {
	typedef struct test_struct {
		unsigned len:28;
		unsigned type:2;
		unsigned valid:1;
	} TestStruct;
	
	TestStruct st;
	std::cout << "sizeof(st): " << sizeof(st)  << std::endl;
	std::cout << "st.len: " << st.len << std::endl;
	std::cout << "st.type: " << st.type << std::endl;
	std::cout << "st.valid: " << st.valid << std::endl;
}

int main() {
	test();
	return 0;
}
