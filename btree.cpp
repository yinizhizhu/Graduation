#include "btree.h"

template<typename keyType>
btree<keyType>::btree() {	//initial
	for (int i = 2 * DEGREE - 1; i >= 0; i--)
		child[i] = NULL;
	leaf = false;
	key_n = 0;
}

template<typename keyType>
void btree<keyType>::setRoot() { leaf = true; }

template<typename keyType>
bool btree<keyType>::search(keyType k) {	//check
	int i = 0;
	while (i < key_n && k > key[i]) i++;
	if (i < key_n && k == key[i]) return true;
	else if (leaf) return false;
	return child[i]->search(k);
}

template<typename keyType>
void btree<keyType>::split(btree* x, int i) {	//split the node
	btree* z = new btree(), *y = x->getC(i);
	z->setL(y->getL());
	z->setN(DEGREE - 1);
	for (int j = 0; j < DEGREE - 1; j++)
		z->setK(j, y->getK(j + DEGREE));
	if (y->leaf == false)
		for (int j = 0; j < DEGREE; j++)
			z->setC(j, y->getC(j + DEGREE));
	y->setN(DEGREE - 1);
	//y->show();//test
	//z->show();//test
	for (int j = x->getN(); j > i; j--)
		x->setC(j + 1, x->getC(j));
	x->setC(i + 1, z);
	for (int j = x->getN() - 1; j >= i; j--)
		x->setK(j + 1, x->getK(j));
	x->setK(i, y->getK(DEGREE - 1));
	x->setN(x->getN() + 1);
	x->show();//test
	cout << endl;//test
}

template<typename keyType>
void btree<keyType>::insertNon(btree* x, keyType k) {	//while NonFull
	int i = x->getN() - 1;
	if (x->leaf) {
		while (i >= 0 && k < x->getK(i)) {
			x->setK(i + 1, x->getK(i));
			i--;
		}
		x->setK(i + 1, k);
		x->setN(x->getN() + 1);
	}
	else {
		while (i >= 0 && k < x->getK(i)) i--;
		i++;
		if (x->getC(i)->getN() == 2 * DEGREE - 1) {
			split(x, i);
			if (k > x->getK(i)) i++;
		}
		insertNon(x->getC(i), k);
	}
}

template<typename keyType>
void btree<keyType>::insert(btree*& root, keyType k) {	//insert the k after spliting
	btree* r = root;
	if (2 * DEGREE - 1 == r->getN()) {
		btree* s = new btree();
		root = s;
		s->setN(0);
		s->setL(false);
		s->setC(0, r);
		split(s, 0);
		insertNon(s, k);
	}
	else insertNon(r, k);
}

template<typename keyType>
void btree<keyType>::merge(btree* x, int i, int j) {
	;
}

template<typename keyType>
void btree<keyType>::del(btree* x, keyType key) {
	;
}

template<typename keyType>
int btree<keyType>::getN() { return key_n; }

template<typename keyType>
bool btree<keyType>::getL() { return leaf; }

template<typename keyType>
keyType btree<keyType>::getK(int i) { return key[i]; }

template<typename keyType>
btree<keyType>* btree<keyType>::getC(int i) { return child[i]; }

template<typename keyType>
void btree<keyType>::setN(int n) { key_n = n; }

template<typename keyType>
void btree<keyType>::setL(bool b) { leaf = b; }

template<typename keyType>
void btree<keyType>::setK(int i, int n) { key[i] = n; }

template<typename keyType>
void btree<keyType>::setC(int i, btree* t) { child[i] = t; }

template<typename keyType>
void btree<keyType>::show() {	//show the nodes in the order of floor
	cout << "(";
	for (int i = 0; i < key_n; i++)
		cout << key[i] << " ";
	cout << ")";
	if (!leaf)
		for (int i = 0; i <= key_n; i++)
			child[i]->show();
}

template<typename keyType>
void btree<keyType>::clear() {	//show the nodes in the order of floor
	if (!leaf)
		for (int i = 0; i <= key_n; i++) {
			child[i]->clear();
			delete child[i];
		}
}