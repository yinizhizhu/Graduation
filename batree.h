#pragma once
#ifndef BATREE_H
#define BATREE_H
#include <iostream>

using namespace std;

#define DEGREEA 3

template<typename keyType>
class batree {
private:
	struct node {
		bool	leaf;					//true while current node is leaf node, false for inner node
		int		key_n;					//the number of the key
		keyType	key[2 * DEGREEA - 1];	//store the key
		node*	child[2 * DEGREEA];		//store the pointer of child
		node() {
			for (int i = 2 * DEGREEA - 1; i >= 0; i--)
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
	bool search(keyType k);								//search k in root
	void split(node* x, int i);							//split the child whose index is i of node x
	void insertNon(node* x, keyType k);					//insert the k into the subtree whose root is node x
	void insert(keyType k);								//insert the k into root
	void merge(node* x, int i, node* y, node* z);		//merge node y, key i and node z, x is the parent of y and z
	void del(keyType k);								//delete the k from root
	void delNon(node* x, keyType k);					//delete the k from the subtree whose root is node x
	void delSet(keyType k, keyType v);					//revalue the index while the head is changed
	void shiftRTL(node* x, int i, node* y, node* z);	//x's right child y borrows a key and a child from x's left child of z
	void shiftLTR(node* x, int i, node* y, node* z);	//...
	void doShow(node* root, int d);
	void show();										//API for showing the btrees
	void doClear(node* root);
	void clear();										//API for free the sources we apply
};
#endif