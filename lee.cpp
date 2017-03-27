#include "btree.cpp"

//struct node {
//	int a, b, c;
//	node(int x, int y, int z) {
//		a = x;
//		b = y;
//		c = z;
//	}
//};
//void test(node*& a) {
//	node* b = new node(6,6,6);
//	a = b;
//}
btree<int> r;

void show() {
	r.show();
	cout << endl;
}

void testInsert(int n) {
	for (int i = 0; i < n; i++) {
		//cout << "Insert " << i << ":" << endl;
		r.insert(i);
		//show();
	}
	show();
}

void testSearch(int n) {
	for (int i = 0; i < n; i++) {
		cout << "Search " << i;
		if (r.search(i))
			cout << " : yes" << endl;
		else
			cout << " : no" << endl;
	}
	r.show();
}

void testDel(int n) {
	r.del(100);//test the case of not exsiting
	char c;
	for (int i = 0; i < n; i++) {
		cout << "Delete " << i << ": " << endl;
		cout << "Continue?(Y/N): ";
		cin >> c;
		if (c != 'Y' && c != 'y') break;
		r.del(i);
		show();
	}
}

void clear() {
	r.clear();
	r.show();
}

int main() {
	//node* a = new node(5,5,5);
	//cout << a->a << " " << a->b << " " << a->c << endl;
	//test(a);
	//cout << a->a << " " << a->b << " " << a->c << endl;

	testInsert(5 * DEGREE);
	testSearch(6 * DEGREE);
	//testDel(5 * DEGREE);
	clear();

	//batree b;
	//b.show();

	return 0;
}