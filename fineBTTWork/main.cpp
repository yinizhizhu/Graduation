#include "batree.cpp"

int main() {
	int i, j;
	for (i = 1; i <= 7; i++) {
		for (j = 0; j < 3; j++) {
			cout << i << " - " << j << "\n";
			batree<int> r(i);
#ifndef TESTBTT
			r.getTree();
#endif
			r.fastRandom();
		}
	}
	return 0;
}
