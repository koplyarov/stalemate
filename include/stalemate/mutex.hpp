#ifndef STALEMATE_MUTEX_HPP
#define STALEMATE_MUTEX_HPP


#include <stalemate/detail/config.hpp>
#include <stalemate/policies.hpp>

#include <iostream>
#include <mutex>
#include <ostream>
#include <thread>


namespace stalemate
{

#if !STALEMATE_PASSTHROUGH

	template < typename LoggerPolicy_, int TimeoutMs_, typename MutexId_ = primitive_id::empty >
	class basic_mutex : private MutexId_
	{
	private:
		std::timed_mutex	_m;

	public:
		constexpr basic_mutex() { }

		template < typename... Args_ >
		constexpr basic_mutex(Args_&&... args) : MutexId_(std::forward<Args_>(args)...) { }

		~basic_mutex() { }

		basic_mutex(const basic_mutex&) = delete;
		basic_mutex& operator = (const basic_mutex&) = delete;

		void lock()
		{
			using namespace std::chrono;

			if (_m.try_lock())
				return;
				LoggerPolicy_::mutex_lock_fail(get_id(), milliseconds(0));

			auto d = milliseconds(TimeoutMs_);
			time_point<steady_clock> tp;
			while (!_m.try_lock_for(d))
				LoggerPolicy_::mutex_lock_fail(get_id(), time_point<steady_clock>() - tp);
		}

		bool try_lock()
		{ return _m.try_lock(); }

		void unlock()
		{ _m.unlock(); }

	private:
		const MutexId_& get_id() const
		{ return *this; }
	};

#else

	template < typename LoggerPolicy_, int TimeoutMs_ >
	using basic_mutex = std::mutex;

#endif

}

#endif
