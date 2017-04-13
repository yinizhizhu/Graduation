#include <thread>
#include <utility>
#include <iostream>

class scoped_thread {
	std::thread t;
public:
	explicit scoped_thread(std::thread t_) : t(std::move(t_)) {
		if (!t.joinable())
			throw std::logic_error("No thread");
	}
	~scoped_thread() {
		t.join();
	}
	scoped_thread(scoped_thread const&) = delete;
	scoped_thread& operator=(scoped_thread const&) = delete;
};

void do_something(int& i) {
	++i;
	std::cout << i << "\n";
}

struct func {
	int& i;
	func(int& i_) : i(i_) {}
	void operator()() {
		for (unsigned j = 0; j < 100; ++j)
			do_something(i);
	}
};

void do_something_in_current_thread() {
	std::cout << "Fucking in current thread!\n";
}

void f() {
	int some_local_state = 0;
	scoped_thread t(std::thread(func(some_local_state)));
	do_something_in_current_thread();
}

int main() {
	f();
}
