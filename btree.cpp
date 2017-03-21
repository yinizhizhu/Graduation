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
bool btree<keyType>::search(btree* x, keyType k) {	//check
	int i = 0, n = x->key_n;
	while (i < n && k > x->key[i++]);
	if (i < n && k == x->key[i]) return true;
	else if (x->leaf) return false;
	return search(x->key[i], k);
}

template<typename keyType>
void btree<keyType>::split(btree* x, int i) {	//split the node
	btree* z = new btree(), *y = x->child[i];
	z->leaf = y->leaf;
	z->key_n = DEGREE - 1;
	for (int j = 0; j < DEGREE - 1; j++)
		z->key[j] = y->key[j + DEGREE];
	if (y->leaf == false)
		for (int j = 0; j < DEGREE; j++)
			z->child[j] = y->child[j + DEGREE];
	y->key_n = DEGREE - 1;
	for (int j = x->key_n - 1; j >= i; j--)
		x->key[j + 1] = x->key[j];
	x->key[i] = y->key[DEGREE - 1];
	x->key_n++;
}

template<typename keyType>
void btree<keyType>::insertNon(btree* x, keyType k) {	//while NonFull
	int i = x->key_n - 1;
	if (x->leaf) {
		while (i >= 0 && k < x->key[i]) {
			x->key[i + 1] = x->key[i];
			i--;
		}
		x->key[i + 1] = k;
		x->key_n++;
	}
	else {
		while (i >= 0 && k < x->key[i]) i--;
		i++;
		if (x->child[i]->key_n == 2 * DEGREE - 1) {
			split(x, i);
			if (k > x->key[i]) i++;
		}
		insertNon(x->child[i], k);
	}
}

template<typename keyType>
void btree<keyType>::insert(btree* root, keyType k) {	//insert the k after spliting
	btree* r = root;
	if (2 * DEGREE - 1 == r->key_n) {
		btree* s = new btree();
		root = s;
		s->leaf = false;
		s->key_n = 0;
		s->child[0] = r;
		split(s, 0);
		insertNon(s, k);
	}
	else insertNon(r, k);
}

template<typename keyType>
void btree<keyType>::del(btree* x, keyType key) {
	;
}

template<typename keyType>
void btree<keyType>::show() {	//show the nodes in the order of floor
	int i, j, n;
	vector<btree*> store;
	store.push_back(this);
	n = 1;
	do {
		for (i = 0; i < n; i++) {
			for (j = 0; j < store[i]->key_n; j++)
				cout << store[i]->key[j] << " ";
			if (store[i]->leaf == false)
				for (j = 0; j <= store[i]->key_n; j++)
					store.push_back(store[i]->child[j]);
			store.erase(store.begin());
		}
		cout << endl;
		n = store.size();
	} while (n);
}
