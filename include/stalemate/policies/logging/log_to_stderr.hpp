#ifndef STALEMATE_POLICIES_LOGGING_LOG_TO_STDERR_HPP
#define STALEMATE_POLICIES_LOGGING_LOG_TO_STDERR_HPP


#include <string>

#include <stdio.h>


namespace stalemate {
namespace logging
{

	struct log_to_stderr
	{
		static void show_message(const std::string& msg)
		{ fprintf(stderr, "%s\n", msg.c_str()); }
	};

}}

#endif
