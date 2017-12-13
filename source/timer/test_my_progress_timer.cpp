#include "my_progress_timer.hpp"
#include "boost/progress.hpp"
#include <cstdio>
#include <sstream>

//#define RE_SHOW 1

void test_my_progress_timer(size_t n) {
	std::cout << "n:" << n << std::endl;
	my_progress_timer<10> timer;
	boost::progress_display display(n);
	for (int i=0; i<n; ++i) {
		for ( int j=0; j<900000; ++j) {}
#ifdef RE_SHOW
		display.restart(n);
		display+=i;
#else
		++display;
#endif
	}
	std::cout << std::endl;
}

int main(int argc, char ** argv) {
	if (argc < 2) {
		return 1;
	}	

	std::stringstream sFmat;
	sFmat << argv[1];
	size_t n;
	sFmat >> n;
	test_my_progress_timer(n);

	return 0;
}
