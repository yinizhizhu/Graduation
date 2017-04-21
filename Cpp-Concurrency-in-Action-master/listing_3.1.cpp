#include <list>
#include <mutex>
#include <algorithm>
#include <iostream>

using namespace std;

list<int> some_list;
mutex some_mutex;

void add_to_list(int new_value) {
	lock_guard<mutex> guard(some_mutex);
	some_list.push_back(new_value);
}

bool list_contains(int value_to_find) {
	lock_guard<mutex> guard(some_mutex);
	return find(some_list.begin(), some_list.end(), value_to_find)
		!= some_list.end();
}

int main() {
	add_to_list(42);
	cout << "contains(1)=" << list_contains(1) << ", contains(42)=" << list_contains(42) << endl;
}

#include <thread>
#include <mutex>
#include <iostream>

int g_i = 0;
std::mutex g_i_mutex;  // protects g_i
void safe_increment() {
	std::lock_guard<std::mutex> lock(g_i_mutex);
	std::cout << std::this_thread::get_id() << ": ";
	std::cout << g_i << ", ";
	++g_i;
	std::cout << g_i << "\n";
	// g_i_mutex is automatically released when lock
	// goes out of scope
}
int main() {
	std::thread t1(safe_increment);
	std::thread t2(safe_increment);
	t1.join();
	t2.join();
}
