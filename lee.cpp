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
btree<int>* root = new btree<int>();

void show() {
	root->show(0);
	cout << endl;
}

void testInsert(int n) {
	for (int i = 0; i < n; i++) {
		//cout << "Insert " << i << ":" << endl;
		root->insert(root, i);
		//show();
	}
	show();
}

void testSearch(int n) {
	for (int i = 0; i < n; i++) {
		cout << "Search " << i;
		if (root->search(i))
			cout << " : yes" << endl;
		else
			cout << " : no" << endl;
	}
}

void testDel(int n) {
	root->del(root, 100);//test the case of not exsiting
	char c;
	for (int i = 0; i < n; i++) {
		cout << "Delete " << i << ": " << endl;
		cout << "Continue?(Y/N): ";
		cin >> c;
		if (c != 'Y' && c != 'y') break;
		root->del(root, i);
		show();
	}
}

void clear() {
	root->clear();
	show();
	delete root;
}

int main() {
	//node* a = new node(5,5,5);
	//cout << a->a << " " << a->b << " " << a->c << endl;
	//test(a);
	//cout << a->a << " " << a->b << " " << a->c << endl;

	root->setRoot();
	testInsert(9 * DEGREE);
	//testSearch(9 * DEGREE);
	testDel(9 * DEGREE);
	clear();

	//batree b;
	//b.show();

	return 0;
}