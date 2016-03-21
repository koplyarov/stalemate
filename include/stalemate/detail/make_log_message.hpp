#ifndef STALEMATE_DETAIL_MAKE_LOG_MESSAGE_HPP
#define STALEMATE_DETAIL_MAKE_LOG_MESSAGE_HPP


#include <stalemate/backtrace.hpp>

#include <chrono>
#include <sstream>


namespace stalemate {
namespace detail
{

	template < typename MutexId_, typename Duration_ >
	std::string make_log_message(const std::string& base_msg, const MutexId_& id, Duration_ d)
	{
		using namespace std::chrono;

		std::stringstream s;
		s << base_msg << " " << id << " for " << duration_cast<milliseconds>(d).count() << " ms, there is probably a deadlock!" << "\n";
		backtrace::print_to_stream(s);

		return s.str();
	}

}}

#endif
