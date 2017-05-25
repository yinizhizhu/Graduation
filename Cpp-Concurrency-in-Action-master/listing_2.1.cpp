#include <thread>
#include <iostream>

using namespace std;

void do_something(int& i) {
	++i;
	cout << i << endl;
}

struct func {
	int& i;
	func(int& i_) : i(i_) {}
	void operator()() {
		for (unsigned j = 0; j<100; ++j)
			do_something(i);
	}
};

void oops() {
	int some_local_state = 0;
	func my_func(some_local_state);
	std::thread my_thread(my_func);
	//my_thread.detach();
	my_thread.join();
}

int main() {
	oops();
}



#include <thread>
#include <iostream>
#include <fstream>

using namespace std;

struct func
{

	func(){}
	void operator()()
	{
		ofstream out("hehe.txt");
		for (unsigned j = 0; j<100; ++j)
		{
			out << j << "\n";
		}
		out.close();
	}
};


void oops()
{
	func my_func;
	thread my_thread(my_func);
	my_thread.detach();
}

int main()
{
	oops();
	for (int i = 0; i < 100000000; i++);
}

