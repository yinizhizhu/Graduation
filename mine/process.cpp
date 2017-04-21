#include <thread>
#include <time.h>
#include <fstream>
#include <Windows.h>
#include <iostream>

using namespace std;

void f1() {
	ofstream out("fuckyou.txt");
	for (int i = 0; i < 100000; i++) {
		out << "Yeah! Fuck you at ";
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		out << sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond << "."<<sys.wMilliseconds << "!\n";
	}
	out << this_thread::get_id() << "\n";
	cout << "fuckyou -> " << this_thread::get_id() << "\n";
	out.close();
}

void f2() {
	ofstream out("fuckme.txt");
	for (int i = 0; i < 1000; i++) {
		out << "Yeah! Fuck me at ";
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		out << sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond << "." << sys.wMilliseconds << "!\n";
	}
	out << this_thread::get_id() << "\n";
	cout << "fuckme -> " << this_thread::get_id() << "\n";
	out.close();
}

int main() {
	thread t1(f1), t2(f2);
	t1.join();
	t2.join();
	cout << this_thread::get_id() << "\n";
	return 0;
}