#ifndef STALEMATE_POLICIES_PRIMITIVE_ID_EMPTY_HPP
#define STALEMATE_POLICIES_PRIMITIVE_ID_EMPTY_HPP


#include <iostream>


namespace stalemate {
namespace primitive_id
{

	class empty
	{
	public:
		empty() { }
		empty(const empty&) = delete;
		empty operator = (const empty&) = delete;
	};


	std::ostream& operator << (std::ostream& os, const empty& id)
	{ return os << (const void*)&id; }

}}

#endif
