#include "my_thread_pool.hpp"
#include "colour.h"
#include <iostream>
#include <iomanip>

boost::mutex g_screen_mutex;

class print_task : public Ctask {
public:
	print_task(int i):m_i(i){}
	~print_task(){}

private:
	void work() 
	{
		boost::unique_lock<boost::mutex> lock(g_screen_mutex);
		std::cout << GREEN << "I'm task" << m_i << " my thread id:" << boost::this_thread::get_id() 
				  << GREEN << std::endl;
		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}
	
private:
	int m_i;
};

 
int main(int argc, char* argv[])
{
	my_thread_pool tp(4, 100);
     
	my_task t[4] = {0};
	int i = 0;
	for (; i<4; ++i) {
		t[i] = new print_task(i);
	}

	i = 0;
	while(true)
	{
		if (!tp.post(t[i]))	
		{
			boost::unique_lock<boost::mutex> lock(g_screen_mutex);
			std::cout  << RED << "add task failed! task overflow!"  << RED << std::endl;
		}
	
		++i;
		if (3 < i) {
			i = 0;
		}
	}

	tp.wait();
	return 0;
}
