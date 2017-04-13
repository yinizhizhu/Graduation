#include <thread>
#include <iostream>

using namespace std;

void some_function() {
	cout << "Fuck you!\n";
}

void some_other_function(int) {
	cout << "Fuck me!\n";
}

thread f() {
	void some_function();
	return thread(some_function);
}
thread g() {
	void some_other_function(int);
	thread t(some_other_function, 42);
	return t;
}

int main() {
	thread t1 = f();
	t1.join();
	thread t2 = g();
	t2.join();
}
