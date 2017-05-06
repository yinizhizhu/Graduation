#include "batree.cpp"

void show(batree<int>& r) {
	r.show();
	cout << endl;
}

void testInsert(batree<int>& r, int n) {
	for (int i = 0; i < n; i++) {
		cout << "Insert " << i << ":" << endl;
		r.insert(i);
		show(r);
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

void clear(batree<int>& r) {
	r.clear();
	r.show();
}

int main() {
	batree<int> r;
	testInsert(r, 9 * DEGREEA);
	testSearch(r, 10 * DEGREEA);
	int n, tmp;
	cout << "The number of test cases: ";
	cin >> n;
	for (; n; n--) {
		cin >> tmp;
		r.del(tmp);
		r.show();
	}
	clear(r);
	return 0;
}