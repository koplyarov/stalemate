#ifndef STALEMATE_MUTEX_HPP
#define STALEMATE_MUTEX_HPP


#include <stalemate/detail/config.hpp>
#include <stalemate/detail/make_log_message.hpp>
#include <stalemate/detail/threading_primitive_guard.hpp>
#include <stalemate/policies.hpp>

#include <mutex>


namespace stalemate
{

#if !STALEMATE_PASSTHROUGH

	template < typename LoggerPolicy_, int TimeoutMs_, typename MutexId_ = primitive_id::empty >
	class basic_mutex : private MutexId_
	{
	private:
		std::timed_mutex	_m;

	public:
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

			detail::threading_primitive_guard g;

			if (g.should_detect_deadlocks())
			{
				auto d = milliseconds(TimeoutMs_);
				detail::timer t;
				while (!_m.try_lock_for(d))
					LoggerPolicy_::show_message(detail::make_log_message("Could not lock mutex", get_id(), t.elapsed()));
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
	class basic_mutex : public std::mutex
	{
	public:
		template < typename... Args_ >
		constexpr basic_mutex(Args_&&... args) { }
	};

#endif

}

#endif
