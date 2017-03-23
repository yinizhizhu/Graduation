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

int main() {
	//node* a = new node(5,5,5);
	//cout << a->a << " " << a->b << " " << a->c << endl;
	//test(a);
	//cout << a->a << " " << a->b << " " << a->c << endl;

	btree<int>* root = new btree<int>();
	root->setRoot();
	for (int i = 0; i < 9 * DEGREE; i++) {
		root->insert(root, i);
		root->show();
		cout << endl;
	}
	for (int i = 0; i < 4 * DEGREE; i++) {
		if (root->search(i))
			cout << i << " : yes" << endl;
		else
			cout << i << " : no" << endl;
	}
	root->clear();
	root->show();
	cout << endl;
	delete root;

	//batree b;
	//b.show();

	return 0;
}