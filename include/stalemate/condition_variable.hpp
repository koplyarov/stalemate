#ifndef STALEMATE_CONDITION_VARIABLE_HPP
#define STALEMATE_CONDITION_VARIABLE_HPP


#include <stalemate/detail/config.hpp>
#include <stalemate/detail/make_log_message.hpp>
#include <stalemate/detail/threading_primitive_guard.hpp>
#include <stalemate/detail/timer.hpp>
#include <stalemate/policies.hpp>

#include <condition_variable>


namespace stalemate
{

#if !STALEMATE_PASSTHROUGH

	template < typename LoggerPolicy_, int TimeoutMs_, typename ConditionVariableId_ = primitive_id::empty >
	class basic_condition_variable : ConditionVariableId_
	{
	private:
		std::condition_variable		_cv;

	public:
		template < typename... Args_ >
		constexpr basic_condition_variable(Args_&&... args) : ConditionVariableId_(std::forward<Args_>(args)...) { }

		void notify_one()
		{ _cv.notify_one(); }

		void notify_all()
		{ _cv.notify_all(); }

		void wait(std::unique_lock<std::mutex>& lock)
		{
			using namespace std::chrono;

			detail::threading_primitive_guard g;

			if (g.should_detect_deadlocks())
			{
				auto d = milliseconds(TimeoutMs_);
				detail::timer t;
				while (_cv.wait_for(lock, d) == std::cv_status::timeout)
					LoggerPolicy_::show_message(detail::make_log_message("Waiting on condition_variable", get_id(), t.elapsed()));
			}
			else
				_cv.wait(lock);
		}

		template< class Predicate_ >
		void wait(std::unique_lock<std::mutex>& lock, Predicate_ pred)
		{
			while (!pred())
				wait(lock);
		}

		template< class Rep_, class Period_ >
		std::cv_status wait_for(std::unique_lock<std::mutex>& lock, const std::chrono::duration<Rep_, Period_>& rel_time)
		{ return _cv.wait_for(lock, rel_time); }

		template< class Rep_, class Period_, class Predicate_ >
		bool wait_for(std::unique_lock<std::mutex>& lock, const std::chrono::duration<Rep_, Period_>& rel_time, Predicate_ pred)
		{ return _cv.wait_for(lock, rel_time, pred); }

		template< class Clock_, class Duration_ >
		std::cv_status wait_until(std::unique_lock<std::mutex>& lock, const std::chrono::time_point<Clock_, Duration_>& abs_time)
		{ return _cv.wait_until(lock, abs_time); }

		template< class Clock_, class Duration_, class Predicate_ >
		bool wait_until(std::unique_lock<std::mutex>& lock, const std::chrono::time_point<Clock_, Duration_>& abs_time, Predicate_ pred)
		{ return _cv.wait_until(lock, abs_time, pred); }

	private:
		const ConditionVariableId_& get_id() const
		{ return *this; }
	};

#else

	template < typename LoggerPolicy_, int TimeoutMs_, typename ConditionVariableId_ = primitive_id::empty >
	class basic_condition_variable : public std::condition_variable
	{
	public:
		template < typename... Args_ >
		constexpr basic_condition_variable(Args_&&... args) { }
	};

#endif

}

#endif
