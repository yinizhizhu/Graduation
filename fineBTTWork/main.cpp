#include "batree.cpp"

int main() {
	batree<int> r;
#ifndef TESTBTT
	r.getTree();
#endif
	r.fastRandom();
	return 0;
}
