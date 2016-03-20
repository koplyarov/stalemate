#ifndef STALEMATE_DETAIL_THREADING_PRIMITIVE_GUARD_HPP
#define STALEMATE_DETAIL_THREADING_PRIMITIVE_GUARD_HPP


namespace stalemate {
namespace detail
{

	class threading_primitive_guard
	{
	private:
		bool _should_detect_deadlocks;

	public:
		threading_primitive_guard()
			: _should_detect_deadlocks(!get_flag())
		{
			if (_should_detect_deadlocks)
				get_flag() = true;
		}

		~threading_primitive_guard()
		{
			if (_should_detect_deadlocks)
				get_flag() = false;
		}

		threading_primitive_guard(const threading_primitive_guard&) = delete;
		threading_primitive_guard operator = (const threading_primitive_guard&) = delete;

		bool should_detect_deadlocks() const
		{ return _should_detect_deadlocks; }

	private:
		static bool& get_flag()
		{
			thread_local bool flag(false);
			return flag;
		}
	};

}}

#endif
