#ifndef _COLOUR_H
#define _COLOUR_H

namespace wanghp {

/*
*	定义了一些控制台彩色输出的宏 
*/
class colour{
public:
	static const char * const RESET		= "\033[0m";
	static const char * const BLACK		= "\033[30m";      /* Black */
	static const char * const RED		= "\033[31m";      /* Red */
	static const char * const GREEN		= "\033[32m";      /* Green */
	static const char * const YELLOW	= "\033[33m";      /* Yellow */
	static const char * const BLUE		= "\033[34m";      /* Blue */
	static const char * const MAGENTA	= "\033[35m";      /* Magenta */
	static const char * const CYAN		= "\033[36m";      /* Cyan */
	static const char * const WHITE		= "\033[37m";      /* White */
	static const char * const BOLDBLACK		= "\033[1m\033[30m";      /* Bold Black */
	static const char * const BOLDRED		= "\033[1m\033[31m";      /* Bold Red */
	static const char * const BOLDGREEN		= "\033[1m\033[32m";      /* Bold Green */
	static const char * const BOLDYELLOW	= "\033[1m\033[33m";      /* Bold Yellow */
	static const char * const BOLDBLUE		= "\033[1m\033[34m";      /* Bold Blue */
	static const char * const BOLDMAGENTA	= "\033[1m\033[35m";      /* Bold Magenta */
	static const char * const BOLDCYAN		= "\033[1m\033[36m";      /* Bold Cyan */
	static const char * const BOLDWHITE		= "\033[1m\033[37m";      /* Bold White */
};

};
#endif//_COLOUR_H
