#include "btree.cpp"

void show(btree<int>& r) {
	r.show();
	cout << endl;
}

void testInsert(btree<int>& r, int n) {
	for (int i = 0; i < n; i++) {
		//cout << "Insert " << i << ":" << endl;
		r.insert(i);
		//show();
	}
	show(r);
}

void testSearch(btree<int>& r, int n) {
	for (int i = 0; i < n; i++) {
		cout << "Search " << i;
		if (r.search(i))
			cout << " : yes" << endl;
		else
			cout << " : no" << endl;
	}
	r.show();
}

void testDel(btree<int>& r, int n) {
	r.del(100);//test the case of not exsiting
	char c;
	for (int i = 0; i < n; i++) {
		cout << "Delete " << i << ": " << endl;
		cout << "Continue?(Y/N): ";
		cin >> c;
		if (c != 'Y' && c != 'y') break;
		r.del(i);
		show(r);
	}
}

void clear(btree<int>& r) {
	r.clear();
	r.show();
}

void testBt() {
	btree<int> r;
	testInsert(r, 5 * DEGREE);
	testSearch(r, 6 * DEGREE);
	testDel(r, 5 * DEGREE);
	clear(r);
}