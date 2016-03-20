#ifndef STALEMATE_DETAIL_IMPL_BACKTRACE_IMPL_HPP
#define STALEMATE_DETAIL_IMPL_BACKTRACE_IMPL_HPP


#include <stalemate/backtrace.hpp>
#include <stalemate/detail/ostream_flags_guard.hpp>

#include <sstream>


#if STALEMATE_BACKTRACE_LIBUNWIND
#	define UNW_LOCAL_ONLY
#	include <libunwind.h>
#elif __GNUC__
#	include <unwind.h>
#endif


namespace stalemate
{

#if STALEMATE_BACKTRACE_LIBUNWIND

	std::ostream& backtrace::print_to_stream(std::ostream& os)
	{
		detail::ostream_flags_guard g(os);

		std::stringstream sym_s;

		unw_cursor_t cursor;
		unw_context_t context;

		unw_getcontext(&context);
		unw_init_local(&cursor, &context);

		while (unw_step(&cursor) > 0)
		{
			unw_word_t offset, pc;
			unw_get_reg(&cursor, UNW_REG_IP, &pc);
			if (pc == 0)
				break;

			os << std::hex << pc << " ";

			char sym[256];
			if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0)
				sym_s << "  (" << sym << "+0x" << std::hex << offset << ")" << std::endl;
			else
				sym_s << "  ( ??? )" << std::endl;
		}

		os << std::endl << sym_s.str();
		return os;
	}

#elif __GNUC__

	namespace
	{
		struct unwind_data
		{
			std::ostream&		os;
			_Unwind_Ptr			last_ip;
			_Unwind_Word		cfa;

			unwind_data(std::ostream& os) : os(os), last_ip(0), cfa(0) { }
		};

		_Unwind_Reason_Code unwind_func(struct _Unwind_Context* ctx, void* user_arg)
		{
			unwind_data* data = static_cast<unwind_data*>(user_arg);

			_Unwind_Ptr ip = _Unwind_GetIP(ctx);
			_Unwind_Word cfa = _Unwind_GetCFA(ctx);

			if (ip == 0 || (data->last_ip == ip && cfa == data->cfa))
				return _URC_END_OF_STACK;

			data->cfa = cfa;
			data->last_ip = ip;

			data->os << std::hex << ip << " ";

			return _URC_NO_REASON;
		}
	}


	std::ostream& backtrace::print_to_stream(std::ostream& os)
	{
		detail::ostream_flags_guard g(os);

		unwind_data data(os);
		_Unwind_Backtrace(&unwind_func, &data);
		std::stringstream sym_s;

		return os;
	}

#else

	std::ostream& backtrace::print_to_stream(std::ostream& os)
	{ return os << "<no backtrace>"; }

#endif

}

#endif
