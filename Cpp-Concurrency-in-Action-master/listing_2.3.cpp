#include <thread>
#include <iostream>

using namespace std;

class thread_guard {
	std::thread& t;
public:
	explicit thread_guard(std::thread& t_) : t(t_) {}
	~thread_guard() {
		if (t.joinable())
			t.join();
	}
	thread_guard(thread_guard const&) = delete;//cannot copy
	thread_guard& operator=(thread_guard const&) = delete;//cannot assign
};

void do_something(int& i) {
	++i;
	cout << i << "\n";
}

struct func {
	int& i;
	func(int& i_) : i(i_) {}
	void operator()() {
		for (unsigned j = 0; j<100; ++j)
			do_something(i);
	}
};

void do_something_in_current_thread() {
	cout << "Fucking in current thread!\n";
}

void f() {
	int some_local_state = 0;
	func my_func(some_local_state);
	std::thread t(my_func);
	thread_guard g(t);
	do_something_in_current_thread();
}

int main() {
	f();
}
