#include <iostream>
#include <thread>
#include <functional>

using namespace std;

class Test {
public:
	Test(int m)	{
		n = m;
	}
	void print() {
		cout << n << '\n';
	}
	~Test() {
		thread testThread_(&Test::print, this);
		if (testThread_.joinable())
			testThread_.join();
	}
private:
	int n;
};

int main() {
	Test test(8);
	return 0;
}