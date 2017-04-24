#include "batree.cpp"

void show(batree<int>& r) {
	r.show();
	cout << endl;
}

void testInsert(batree<int>& r, int n) {
	for (int i = 0; i < n; i++) {
		//cout << "Insert " << i << ":" << endl;
		r.insert(i);
		//show(r);
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
	//char c;
	for (int i = 0; i < n; i++) {
		cout << "Delete " << i << ": " << endl;
		//cout << "Continue?(Y/N): ";
		//cin >> c;
		//if (c != 'Y' && c != 'y') break;
		r.del(i);
		show(r);
	}
}

int main() {
	cout << "Main thread_id: " << this_thread::get_id() << "\n";
	batree<int> r;
	r.getTree();
	//testInsert(r, 9 * DEGREE);
	//testSearch(r, 10 * DEGREE);
	//r.palm();
	r.test(18);
	//testDel(r, 9 * DEGREE + 1);
	return 0;
}