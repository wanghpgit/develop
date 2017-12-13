#include <iostream>
#include <string>

class screen {
public:
	screen(const std::string & name, int x, int y):_name(new std::string(name)), _x(x), _y(y) {}
	screen(const screen & that) :_name(new std::string(*that._name)), _x(that._x), _y(that._y) {}
	~screen() { delete _name; }
	void show() const {}

	std::string * _name;
	int _x, _y;
};

class screenPtr;
class scrPtr{
	friend class screenPtr;
	screen * _sp;
	size_t _use;
	scrPtr(screen * ptr):_sp(ptr), _use(1){}
	~scrPtr(){delete _sp;}
};

class screenPtr{
public:
	screenPtr(screen * p):_ptr(new scrPtr(p)){
		std::cout << "screenPtr()" << std::endl;
	}
	~screenPtr(){
		std::cout << "~screenPtr()" << std::endl;
		if (0 == --_ptr->_use) {
			std::cout << "~screenPtr() delete" << std::endl;
			delete _ptr;
		}
	}

	screenPtr(const screenPtr & that):_ptr(that._ptr) { ++_ptr->_use; }

	inline screen & operator*(){
		std::cout << "screen & operator*()" << std::endl;
		return *_ptr->_sp;
	}

	inline const screen & operator*() const {
		std::cout << "const screen & operator*() const" << std::endl;
		return *_ptr->_sp;
	}

	inline screen * operator->(){
		std::cout << "screen * operator->()" << std::endl;
		return _ptr->_sp;
	}

	inline const screen * operator->() const {
		std::cout << "const screen * operator->() const" << std::endl;
		return _ptr->_sp;
	}

private:
	scrPtr * _ptr;
};

void test_ptr(){
	screen scr("123456",9, 100);
	screenPtr ptr1(new screen(scr));
	const screenPtr ptr2 = ptr1;
	ptr1->show();
	ptr1->_name->length();
	(*ptr1)._x;
	ptr2->show();
	ptr2->_name->length();
	(*ptr2)._x;
}

int main(){
	test_ptr();
	return 0;
}
