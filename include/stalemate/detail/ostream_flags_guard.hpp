#ifndef STALEMATE_DETAIL_OSTREAM_FLAGS_GUARD_HPP
#define STALEMATE_DETAIL_OSTREAM_FLAGS_GUARD_HPP


#include <iostream>


namespace stalemate {
namespace detail
{

	class ostream_flags_guard
	{
		std::ostream&		_os;
		std::ios::fmtflags	_f;

	public:
		ostream_flags_guard(std::ostream& os) : _os(os), _f(os.flags()) { }
		~ostream_flags_guard() { _os.flags(_f); }
	};

}}

#endif
