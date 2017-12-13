#include <iostream>

void test_ptr_referenc(){
	std::string * ptrStr = new std::string("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	std::string & refStr = *ptrStr;
	std::cout << "ptrStr:" << std::hex << ptrStr << "\t" << *ptrStr << std::endl;
	std::cout << "refStr:" << std::hex << &refStr << "\t" << refStr << std::endl;
	delete &refStr;
}

class friend_class;
class base_class {
public:
	base_class():m_str1("base_class protected: m_str1"), m_str2("base_class private: m_str2"){
		std::cout << "base_class::base_class()" << std::endl;
	}
	virtual ~base_class(){
		std::cout << "base_class::~base_class()" << std::endl;
	}
	virtual void print(){
		std::cout << "base_class::print()" << std::endl;
	}

	void func(){
		std::cout << "base_class::func()" << std::endl;
	}

	friend void class_print(base_class & ref){
		std::cout << "class_print\t m_str1: " << ref.m_str1 << "\tm_str2: " << ref.m_str2 << std::endl;
	}
	friend class friend_class;
protected:

	std::string m_str1;

private:
	std::string m_str2;
};

class child_class : public base_class {
public:
	child_class(): m_str3("child_class protected: m_str3"), m_str4("child_class private: m_str4") {
		std::cout << "child_class::child_class()" << std::endl;
	}
	~child_class() {
		std::cout << "child_class::~child_class()" << std::endl;
	}

	void print(){
		std::cout << "child_class::print()" << std::endl;
	}

	void func(){
		std::cout << "child_class::func()" << std::endl;
	}


protected:
	std::string m_str3;
private:
	std::string m_str4;
};

class friend_class {
public:
	friend_class(){}
	~friend_class(){}
	void print(const base_class & ref){
		std::cout << "friend_class print\t m_str1: " << ref.m_str1 << "\tm_str2: " << ref.m_str2 << std::endl;
	}
	void print(const child_class & ref){
		print((base_class&)ref);
//		std::cout << "friend_class print\t m_str13 " << ref.m_str3 << "\tm_str4: " << ref.m_str4 << std::endl;
		/*
			错误！ friend_class是base_class的友员。friend_class可以访问base_class的m_str1和tm_str2。但是友员关系不能继承。
			所以friend_class不是child_class的友员。friend_class不能访问child_class的m_str3和m_str4。
		*/
	}
};

void test_inherit_class() {
	base_class * ptr = new child_class;
	ptr->print();
	ptr->func();
	delete ptr;
	std::cout << "--------------------------------------" << std::endl;
	base_class & ref = *(new child_class);
	ref.print();
	ref.func();
	delete &ref;
	std::cout << "--------------------------------------" << std::endl;
	base_class base;
	child_class child;
	friend_class fri;
	class_print(base);
	class_print(child);
	fri.print(base);
	fri.print(child);
	base.func();
	child.func();
}

int main() {
	std::cout << "######################################" << std::endl;
	test_ptr_referenc();
	std::cout << "######################################" << std::endl;
	test_inherit_class();
	std::cout << "######################################" << std::endl;
	return 0;
}