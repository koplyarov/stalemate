#ifndef STALEMATE_RECURSIVE_MUTEX_HPP
#define STALEMATE_RECURSIVE_MUTEX_HPP


#include <stalemate/detail/config.hpp>
#include <stalemate/detail/make_log_message.hpp>
#include <stalemate/detail/threading_primitive_guard.hpp>
#include <stalemate/policies.hpp>

#include <mutex>
#include <thread>


namespace stalemate
{

#if !STALEMATE_PASSTHROUGH

	template < typename LoggerPolicy_, int TimeoutMs_, typename MutexId_ = primitive_id::empty >
	class basic_recursive_mutex : private MutexId_
	{
	private:
		std::recursive_timed_mutex	_m;

	public:
		template < typename... Args_ >
		constexpr basic_recursive_mutex(Args_&&... args) : MutexId_(std::forward<Args_>(args)...) { }

		~basic_recursive_mutex() { }

		basic_recursive_mutex(const basic_recursive_mutex&) = delete;
		basic_recursive_mutex& operator = (const basic_recursive_mutex&) = delete;

		void lock()
		{
			using namespace std::chrono;

			if (_m.try_lock())
				return;

			detail::threading_primitive_guard g;

			if (g.should_detect_deadlocks())
			{
				auto d = milliseconds(TimeoutMs_);
				detail::timer t;
				while (!_m.try_lock_for(d))
					LoggerPolicy_::show_message(detail::make_log_message("Could not lock recursive_mutex", get_id(), t.elapsed()));
			}
			else
				_m.lock();
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

	template < typename LoggerPolicy_, int TimeoutMs_, typename MutexId_ = primitive_id::empty >
	class basic_recursive_mutex : public std::recursive_mutex
	{
	public:
		template < typename... Args_ >
		constexpr basic_recursive_mutex(Args_&&... args) { }
	};

#endif

}

#endif
