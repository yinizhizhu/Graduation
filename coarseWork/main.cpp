#include "batree.cpp"

int main() {
	int i, j;
	for (i = 1; i <= 7; i++) {
		for (j = 0; j < 3; j++) {
			batree<int> r(i);
			r.getTree();
			r.fastRandom();
		}
	}
	return 0;
}
