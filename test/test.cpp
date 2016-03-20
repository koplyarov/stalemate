#include <stalemate/backtrace.hpp>
#include <stalemate/mutex.hpp>
#include <stalemate/amalgamation.hpp>

#include <sstream>
#include <thread>

#include <stdio.h>


using namespace stalemate;

namespace test
{
	using mutex = stalemate::basic_mutex<logging::log_to_stderr, 1000, primitive_id::owner_type>;

	class A
	{
	private:
		mutable mutex	_m;

	public:
		A() : _m(this) { }

		mutex& get_mutex() const { return _m; }
	};
}

void f()
{
	test::A a;

	std::thread t([&] {
			std::lock_guard<decltype(a.get_mutex())> l(a.get_mutex());
			std::this_thread::sleep_for(std::chrono::seconds(3));
		});

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	{
		std::lock_guard<decltype(a.get_mutex())> l(a.get_mutex());
	}

	t.join();
}


void g()
{
	f();
}


int main()
{
	g();
	return 0;
}
