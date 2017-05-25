#include "batree.cpp"

int main() {
	batree<int> r;
	r.getTree();
	r.fastRandom();
	r.show();
	r.testParent(36);
	return 0;
}
