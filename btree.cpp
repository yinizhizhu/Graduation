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
void btree<keyType>::merge(btree* x, int i, btree* y, btree* z) {
	//i: the index of key in x, y: left child of x, z: right child of x
	int j = DEGREE;
	y->setN(2 * DEGREE - 1);
	for (; j < 2 * DEGREE - 1; j++)
		y->setK(j, z->getK(j - DEGREE));
	y->setK(DEGREE - 1, x->getK(i));
	if (!y->getL())
		for (j = DEGREE; j < 2 * DEGREE; j++)
			y->setC(j, z->getC(j - DEGREE));
	for (j = i + 1; j < x->getN() - 1; j++) {
		x->setK(j - 1, x->getK(j));
		x->setC(j, x->getC(j + 1));
	}
	x->setN(x->getN() - 1);
	delete z;
}

template<typename keyType>
void btree<keyType>::del(btree*& root, keyType k) {
	btree* r = root;
	if (r->getN() == 1 && !r->getL()) {
		btree* y = root->getC(0);
		btree* z = root->getC(1);
		if (y->getN() == z->getN() && z->getN() == DEGREE - 1) {
			merge(r, 0, y, z);
			root = y;
			delete r;
			delNon(y, k);
		}
		else delNon(r, k);
	}
	else delNon(r, k);
}

template<typename keyType>
void btree<keyType>::delNon(btree* x, keyType k) {
	int i = 0;
	while (i < x->getN() && k > x->getK(i)) i++;
	if (x->getL()) {
		if (k == x->getK(i)) {
			for (int j = i + 1; j < x->getN(); j++)
				x->setK(j - 1, x->getK(j));
			x->setN(x->getN() - 1);
		}
		else cout << "The key does not exist!" << endl;
	}
	else {
		keyType ans;
		btree* z = NULL, *y = x->getC(i);
		if (i < x->getN()) {
			z = x->getC(i + 1);
			if (k == x->getK(i)) {
				if (y->getN() >= DEGREE) {
					ans = searchPre(y);
					delNon(z, ans);
					x->setK(i, ans);
				}
				else if (z->getN() >= DEGREE) {
					ans = searchSuc(z);
					delNon(z, ans);
					x->setK(i, ans);
				}
				else {
					merge(x, i, y, z);
					delNon(y, ans);
				}
			}
			else {
				btree* p = NULL;
				if (i > 0) p = x->getC(i - 1);
				if (y->getN() == DEGREE - 1) {
					if (i > 0 && p->getN() >= DEGREE)
						shiftRTL(x, i, p, y);
					else if (i < x->getN() && z->getN() >= DEGREE - 1)
						shiftLTR(x, i, y, z);
					else if (i >= 1) {
						merge(x, i, p, y);
						y = p;
					}
					else merge(x, i, y, z);
					delNon(y, k);
				}
			}
		}
		else delNon(y, k);
	}
}

template<typename keyType>
keyType btree<keyType>::searchPre(btree* y) {
	btree* x = y;
	while (!x->getL())
		x = x->getC(x->getN());
	return x->getK(x->getN() - 1);
}

template<typename keyType>
keyType btree<keyType>::searchSuc(btree* z) {
	btree* x = z;
	while (!x->getL()) x = x->getC(0);
	return x->getK(0);
}

template<typename keyType>
void btree<keyType>::shiftRTL(btree* x, int i, btree* y, btree* z) {
	//i: the index of key in x, y: left child of x, z: right child of x
	int j = z->getN();
	z->setN(j + 1);
	for (; j >= 0; j--)
		z->setK(j, z->getK(j - 1));
	z->setK(0, x->getK(i));
	x->setK(i, y->getK(y->getN() - 1));
	if (!z->getL()) {
		for (j = z->getN(); j >= 0; j--)
			z->setC(j + 1, z->getC(j));
		z->setC(0, y->getC(y->getN()));
	}
	y->setN(y->getN() - 1);
}

template<typename keyType>
void btree<keyType>::shiftLTR(btree* x, int i, btree* y, btree* z) {
	//i: the index of key in x, y: left child of x, z: right child of x
	int j = y->getN();
	y->setN(j + 1);
	y->setK(j, x->getK(i));
	for (j = 1; j < z->getN(); j++)
		z->setK(j - 1, z->getK(j));
	if (!z->getL()) {
		y->setC(y->getN(), z->getC(0));
		for (j = 1; j <= z->getN(); j++)
			z->getC(j - 1, z->getC(j));
	}
	z->setN(z->getN() - 1);
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
