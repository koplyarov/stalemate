#ifndef STALEMATE_DETAIL_TIMER_HPP
#define STALEMATE_DETAIL_TIMER_HPP


#include <chrono>


namespace stalemate {
namespace detail
{

	class timer
	{
		using clock_type = std::chrono::steady_clock;
		using time_point_type = std::chrono::time_point<clock_type>;

	private:
		time_point_type		_tp;

	public:
		timer()
			: _tp(clock_type::now())
		{ }

		auto elapsed() const -> decltype(time_point_type() - time_point_type())
		{ return clock_type::now() - _tp; }
	};

}}

#endif
