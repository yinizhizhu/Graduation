#include <mutex>
#include <string>
#include <iostream>

using namespace std;

class some_data {
	int a;
	string b;
public:
	some_data() : a(0), b("abc") {}
	void do_something() {
		cout << a << " " << b << "\n";
	}
};

class data_wrapper {
private:
	some_data data;
	mutex m;
public:
	template<typename Function>
	void process_data(Function func) {
		lock_guard<mutex> l(m);
		func(data);
	}
};

some_data* unprotected;

void malicious_function(some_data& protected_data) {
	unprotected = &protected_data;
}

data_wrapper x;

void foo() {
	x.process_data(malicious_function);
	unprotected->do_something();
}

int main() {
	foo();
}
