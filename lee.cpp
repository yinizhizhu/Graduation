#include "btree.cpp"

int main() {
	btree<int>* root = new btree<int>();
	root->setRoot();
	for (int i = 0; i < 3 * DEGREE - 1; i++) {
		root->insert(root, i);
		root->show();
		cout << endl;
	}
	//batree b;
	//b.show();
	return 0;
}