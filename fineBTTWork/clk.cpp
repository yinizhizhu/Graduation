#include "clk.h"

my_clock::my_clock() {
	QueryPerformanceFrequency(&freq);
}

my_clock::~my_clock() {}

void my_clock::start() {
	QueryPerformanceCounter(&start_t);
}

double my_clock::stop() {
	QueryPerformanceCounter(&stop_t);
	return (1e3*(stop_t.QuadPart - start_t.QuadPart) / freq.QuadPart);
}