#include "batree.cpp"

void show(batree<int>& r) {
	r.show();
	cout << endl;
}

void testInsert(batree<int>& r, int n) {
	for (int i = 0; i < n; i++) {
		//cout << "Insert " << i << ":" << endl;
		r.insert(i);
		//show();
	}
	show(r);
}

void testSearch(batree<int>& r, int n) {
	for (int i = 0; i < n; i++) {
		cout << "Search " << i;
		if (r.search(i))
			cout << " : yes" << endl;
		else
			cout << " : no" << endl;
	}
	r.show();
}

void testDel(batree<int>& r, int n) {
	r.del(100);//test the case of not exsiting
	for (int i = 0; i < n; i++) {
		cout << "Delete " << i << ": " << endl;
		r.del(i);
		show(r);
	}
}

int main() {
	batree<int> r;
	testInsert(r, 9 * DEGREE);
	//testSearch(r, 10 * DEGREE);
	//testDel(r, 9 * DEGREE);
	r.fastRandom();
	return 0;
}