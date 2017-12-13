#include "my_thread.hpp"
#include <iostream>

class print_thread: public my_thread {
public:

protected:
	virtual void work() throw(boost::thread_interrupted){
		while ( true ) {
				std::cout << boost::this_thread::get_id() << std::endl;
				boost::this_thread::sleep(boost::posix_time::seconds(1));
		}
	}
};


int main() {
	print_thread th;
	th.start(false);
	signal(SIGINT, &print_thread::signal_cbf);
	signal(SIGHUP, &print_thread::signal_cbf);
	signal(SIGQUIT, &print_thread::signal_cbf);
	
	while(1){
	}
	return 0;
}
