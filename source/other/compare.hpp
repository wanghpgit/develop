#ifndef __COMPARE_H
#define __COMPARE_H

namespace wanghp {

/*
 *	比较操作类
 *	前提条件：左右操作数支持"=="和"<"操作符
*/
template <typename L, typename R = L>
class compare {
public:
	compare(){
	}
	
	~compare() {

	}

	/*等于 == */
	static bool eq(const L & left, const R & right) {
		return (left == right) ? true : false ;
	}

	/*不等于 != */
	static bool ne(const L & left, const R & right) {
		return !eq(left, right);
	}

	/*大于 > */
	static bool gt(const L & left, const R & right) {
		return (ge(left, right)  && ne(left, right)) ? true : false;	
	}

	/*大于等于 >= */
	static bool ge(const L & left, const R & right) {
		return !lt(left, right);
	}

	/*小于 < */
	static bool lt(const L & left, const R & right) {
		return left < right;
	}

	/*小于等于 <= */
	static bool le(const L & left, const R & right) {
		return !gt(left, right);
	}

};

}

#endif//__COMPARE_H
