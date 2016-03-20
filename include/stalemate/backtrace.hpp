#ifndef STALEMATE_BACKTRACE_HPP
#define STALEMATE_BACKTRACE_HPP


#include <array>
#include <iostream>


namespace stalemate
{

	class backtrace
	{
	public:
		static std::ostream& print_to_stream(std::ostream& os);
	};

}

#endif
