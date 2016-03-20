#ifndef STALEMATE_POLICIES_LOGGING_LOG_TO_STDERR_HPP
#define STALEMATE_POLICIES_LOGGING_LOG_TO_STDERR_HPP


#include <stalemate/backtrace.hpp>

#include <chrono>
#include <sstream>

#include <stdio.h>


namespace stalemate {
namespace logging
{

	struct log_to_stderr
	{
		template < typename MutexId_, typename Duration_ >
		static void mutex_lock_fail(const MutexId_& id, Duration_ d)
		{
			using namespace std::chrono;

			std::stringstream s;
			s << "Could not lock mutex " << id << " for " << duration_cast<milliseconds>(d).count() << " ms, there is probably a deadlock!" << "\n";
			backtrace::print_to_stream(s);

			fprintf(stderr, "%s\n", s.str().c_str());
		}
	};

}}

#endif
