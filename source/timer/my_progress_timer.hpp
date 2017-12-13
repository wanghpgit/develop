#include "boost/progress.hpp"
#include "boost/static_assert.hpp"
#include <iostream>

/*
* 可以指定精度，默认小数点2位 
*/
template <int precision = 2>
class my_progress_timer : public boost::timer {
public:
	my_progress_timer(std::ostream & os = std::cout): m_os(os) {
		BOOST_STATIC_ASSERT((precision >= 0) && (precision <= 10)); 
	}

	~my_progress_timer() {
		try {
			std::istream::fmtflags old_flags = m_os.setf(std::istream::fixed, std::istream::floatfield);
			std::streamsize old_prec = m_os.precision(precision);
			m_os << elapsed() << " s" << std::endl;
			m_os.flags(old_flags);
			m_os.precision(old_prec);
		}catch(...){
		}
		
	}
private:
	std::ostream & m_os;
};
