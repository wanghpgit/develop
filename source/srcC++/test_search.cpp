#include <list>
#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <string>

void test_search(){
	std::deque<int> coll;
	std::list<int> collSub;

	for (int j=0; j<3; ++j){
		for (int i=1;i<101;++i){
			coll.push_back(i);
		}
	}

	for (int i=10; i<20; ++i){
		collSub.push_back(i);
	}
	

	std::cout << "coll:\n";
	std::copy(coll.begin(), coll.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout <<"\n\n\ncollSub:\n";
	std::copy(collSub.begin(), collSub.end(), std::ostream_iterator<int>(std::cout, " "));

	std::deque<int>::iterator it = std::search(coll.begin(), coll.end(), collSub.begin(), collSub.end());
	std::cout << "\n\n\nsearch end:\n";
	std::copy(it, coll.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << "\n\n\nfind_end:\n";
	it = std::find_end(coll.begin(), coll.end(), collSub.begin(), collSub.end());
	std::cout << std::endl;
}

template <class T>
void insert_elements(T & coll, int beg, int end){
	for (; beg<=end; ++beg){
		coll.push_back(beg);
	}
}

template <class T>
void print_elements(T  & coll, const std::string & str){
	std::cout << str;
	std::copy(coll.begin(), coll.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;
}

int main(){
	//test_search();
	std::deque<int> coll_deque;
	std::cout << "deque" << std::endl;
	insert_elements<std::deque<int> >(coll_deque, 1, 100);
	std::cout << "coll.size: " << coll_deque.size() << std::endl;
	print_elements<std::deque<int> >(coll_deque, "xxxxxxxxxxxxxxxx: ");

	std::list<int> coll_list;
	std::cout << "list" << std::endl;
	insert_elements<std::list<int> >(coll_list, 1, 100);
	std::cout << "coll.size: " << coll_list.size() << std::endl;
	print_elements<std::list<int> >(coll_list, "sssssssssssss: ");


	return 0;
}