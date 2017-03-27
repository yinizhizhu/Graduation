#pragma once
#ifndef BATREE_H
#define BATREE_H
#include <iostream>

using namespace std;

#define DEGREE 3

template<typename keyType>
class batree {
private:
	struct node {
		bool	leaf;
		int		key_n;
		keyType	key[2 * DEGREE - 1];
		node*	child[2 * DEGREE];
		node() {
			for (int i = 2 * DEGREE - 1; i >= 0; i--)
				child[i] = NULL;
			leaf = false;
			key_n = 0;
		}
		int getN() { return key_n; }
		bool getL() { return leaf; }
		keyType getK(int i) { return key[i]; }
		node* getC(int i) { return child[i]; }
		void setN(int n) { key_n = n; }
		void setL(bool b) { leaf = b; }
		void setK(int i, int n) { key[i] = n; }
		void setC(int i, node* t) { child[i] = t; }
	};
	node* root;
public:
	batree();
	~batree();
	bool search(keyType k);
	void split(node* x, int i);
	void insertNon(node* x, keyType k);
	void insert(keyType k);
	void merge(node* x, int i, node* y, node* z);
	void del(keyType k);
	void delNon(node* x, keyType k);
	void shiftRTL(node* x, int i, node* y, node* z);
	void shiftLTR(node* x, int i, node* y, node* z);
	void doShow(node* root, int d);
	void show();
	void doClear(node* root);
	void clear();
};
#endif