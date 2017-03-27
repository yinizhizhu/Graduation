#include "batree.h"

template<typename keyType>
batree<keyType>::batree() {	//initial
	root = new node();
	root->setL(true);
}

template<typename keyType>
batree<keyType>::~batree() {	//initial
	clear();
	delete root;
}

template<typename keyType>
bool batree<keyType>::search(keyType k) {	//check
	node* tmp = root;
	while (tmp) {
		int i = 0;
		while (i<tmp->getN() && k > tmp->getK(i)) i++;
		if (i < tmp->getN() && k == tmp->getK(i)) return true;
		else if (tmp->getL()) return false;
		tmp = tmp->getC(i);
	}
	return false;
}

template<typename keyType>
void batree<keyType>::split(node* x, int i) {	//split the node
	int len = DEGREE - 1, basis = 0;//for the iner node
	node* z = new node(), *y = x->getC(i);
	z->setL(y->getL());
	if (z->getL()) {//for the leaf node
		len++;
		basis = -1;
	}
	z->setN(len);
	for (int j = 0; j < len; j++)
		z->setK(j, y->getK(j + DEGREE + basis));
	if (y->leaf == false)
		for (int j = 0; j <= len; j++)
			z->setC(j, y->getC(j + DEGREE + basis));
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
	//x->show(0);//test
	//cout << endl;//test
}

template<typename keyType>
void batree<keyType>::insertNon(node* x, keyType k) {	//while NonFull
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
void batree<keyType>::insert(keyType k) {	//insert the k after spliting
	node* r = root;
	if (2 * DEGREE - 1 == r->getN()) {
		node* s = new node();
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
void batree<keyType>::merge(node* x, int i, node* y, node* z) {
	//i: the index of key in x, y: left child of x, z: right child of x
	int j = DEGREE, basis = 0;
	if (y->getL()) {
		j--;
		basis++;
	}
	y->setN(2 * DEGREE - 1);
	for (; j < 2 * DEGREE - 1; j++)
		y->setK(j, z->getK(j - DEGREE + basis));
	if (!y->getL()) {
		y->setK(DEGREE - 1, x->getK(i));
		for (j = DEGREE; j < 2 * DEGREE; j++)
			y->setC(j, z->getC(j - DEGREE));
	}
	for (j = i + 1; j < x->getN(); j++) {
		x->setK(j - 1, x->getK(j));
		x->setC(j, x->getC(j + 1));
	}
	x->setN(x->getN() - 1);
	delete z;
}

template<typename keyType>
void batree<keyType>::del(keyType k) {
	if (search(k)) {
		node* r = root;
		if (r->getN() == 1 && !r->getL()) {
			node* y = root->getC(0);
			node* z = root->getC(1);
			if ((!y->getL() && y->getN() + z->getN() == 2 * DEGREE - 2)
				|| (y->getL() && y->getN() + z->getN() == 2 * DEGREE - 1)) {
				merge(r, 0, y, z);
				root = y;
				delete r;
				delNon(y, k);
			}
			else delNon(r, k);
		}
		else delNon(r, k);
	}
}

template<typename keyType>
void batree<keyType>::delNon(node* x, keyType k) {
	int i = 0;
	while (i < x->getN() && k > x->getK(i)) i++;
	if (x->getL()) {//Reach the leaf node
		if (k == x->getK(i)) {
			for (int j = i + 1; j < x->getN(); j++)
				x->setK(j - 1, x->getK(j));
			x->setN(x->getN() - 1);
		}
		else cout << "The key does not exist!" << endl;
		return;
	}
	// the iner node
	node* z = NULL, *y = x->getC(i);
	if (i < x->getN()) z = x->getC(i + 1);
	if (k == x->getK(i)) {
		if (y->getN() >= DEGREE) {//get the key from the left node
			delNon(y, k);
		}
		else if (z->getN() >= DEGREE) {//get the key from the right node
			delNon(z, k);
		}
		else {//merge the y, k, z
			merge(x, i, y, z);
			delNon(y, k);
		}
	}
	else {
		node* p = NULL;
		if (i > 0) p = x->getC(i - 1);
		if (y->getN() == DEGREE - 1) {
			if (i > 0 && p->getN() >= DEGREE)//Get: try the left side
				shiftRTL(x, i - 1, p, y);
			else if (i < x->getN() && z->getN() >= DEGREE)//Get: try the right side
				shiftLTR(x, i, y, z);
			else if (i > 0) {//Merge: try the left side
				merge(x, i - 1, p, y);
				y = p;
			}
			else merge(x, i, y, z);//Merge: try the right side
		}
		delNon(y, k);
	}
}

template<typename keyType>
void batree<keyType>::shiftRTL(node* x, int i, node* y, node* z) {
	//i: the index of key in x, y: left child of x, z: right child of x
	int j = z->getN();
	for (; j > 0; j--)
		z->setK(j, z->getK(j - 1));
	z->setK(0, x->getK(i));
	x->setK(i, y->getK(y->getN() - 1));
	if (!z->getL()) {
		for (j = z->getN(); j >= 0; j--)
			z->setC(j + 1, z->getC(j));
		z->setC(0, y->getC(y->getN()));
	}
	z->setN(z->getN() + 1);
	y->setN(y->getN() - 1);
}

template<typename keyType>
void batree<keyType>::shiftLTR(node* x, int i, node* y, node* z) {
	//i: the index of key in x, y: left child of x, z: right child of x
	int n = y->getN();
	y->setK(n, x->getK(i));
	x->setK(i, z->getK(0));
	for (int k = 1; k < z->getN(); k++)
		z->setK(k - 1, z->getK(k));
	y->setC(n + 1, z->getC(0));
	if (!z->getL())
		for (int k = 1; k <= z->getN(); k++)
			z->setC(k - 1, z->getC(k));
	y->setN(n + 1);
	z->setN(z->getN() - 1);
}

template<typename keyType>
void batree<keyType>::doShow(node* root, int d) {	//show the nodes in the order of floor
	node* tmp = root;
	for (int i = 0; i < d; i++) cout << "   ";
	if (d) cout << "->";
	cout << "(" << tmp->getN() << ": ";
	for (int i = 0; i < tmp->getN(); i++)
		cout << tmp->getK(i) << " ";
	cout << ")" << endl;
	if (!tmp->getL())
		for (int i = 0; i <= tmp->getN(); i++)
			doShow(tmp->getC(i), d + 1);
}

template<typename keyType>
void batree<keyType>::show() {
	doShow(root, 0);
}

template<typename keyType>
void batree<keyType>::doClear(node* root) {	//show the nodes in the order of floor
	if (!root->getL())
		for (int i = 0; i <= root->getN(); i++) {
			doClear(root->getC(i));
			root->setL(true);
			delete root->getC(i);
		}
}

template<typename keyType>
void batree<keyType>::clear() {
	doClear(root);
}