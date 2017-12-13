#include "boost/thread.hpp"
#include "boost/scoped_ptr.hpp"
#include <signal.h>
#ifdef __DEBUG
#include <iostream>
#endif

class my_thread {
public:
	my_thread():m_thread(NULL) {
	}

	~my_thread() {
	}
	bool start(bool isJoin=false) {
		if(NULL != m_thread) {
			return false;
		}

		m_thread.reset(new boost::thread(boost::bind(&my_thread::thread_function, this)));
		
		
		if(isJoin) {
			m_thread->join();
		}

		return true;
	}	

	static void signal_cbf(int signal) {
		switch(signal) {
			case SIGINT:
			case SIGHUP:
			case SIGQUIT:
#ifdef __DEBUB
				std::cout << "stop this thread" << std::endl;
#endif
				boost::this_thread::interruption_point();
				break;
		}
	}

protected:
	virtual void work() throw(boost::thread_interrupted){
		while ( true ) {
			
		}
	}

private:
	void thread_function() {
		try {
			work();
		} catch (boost::thread_interrupted &) {
			
		}
	}

	 boost::scoped_ptr<boost::thread>  m_thread;
};
