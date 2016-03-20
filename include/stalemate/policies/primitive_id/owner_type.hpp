#ifndef STALEMATE_POLICIES_PRIMITIVE_ID_OWNER_TYPE_HPP
#define STALEMATE_POLICIES_PRIMITIVE_ID_OWNER_TYPE_HPP


#include <iostream>
#include <typeinfo>


namespace stalemate {
namespace primitive_id
{

	class owner_type
	{
	private:
		const std::type_info*		_t;

	public:
		owner_type() : _t(nullptr) { }

		template < typename T_ >
		owner_type(const T_*) : _t(&typeid(T_)) { }

		owner_type(const owner_type&) = delete;
		owner_type operator = (const owner_type&) = delete;

		const std::type_info* get_type_info() const { return _t; }
	};


	std::ostream& operator << (std::ostream& os, const owner_type& id)
	{
		if (id.get_type_info())
			os << id.get_type_info()->name() << "::";
		return os << (const void*)&id;
	}

}}

#endif
