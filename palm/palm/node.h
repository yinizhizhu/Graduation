#ifndef NODE_H
#define NODE_H
#pragma once

#include <iostream>
using namespace std;

#define	DEGREE	3
#define MIN_DEGREE DEGREE-1
#define MAX_DEGREE 2*DEGREE-1
typedef unsigned int INDEX;

template<typename keyType>
struct node {
	bool	leaf;					//true while current node is leaf node, false for inner node
	int		key_n;					//the number of the key
	keyType	key[MAX_DEGREE];	//store the key
	node*	parent;
	node*	child[MAX_DEGREE + 1];		//store the pointer of child
	node(node* p = NULL, int k_n = 0, bool l = false) {
		parent = p;
		for (int i = MAX_DEGREE; i >= 0; i--)
			child[i] = NULL;
		leaf = l;
		key_n = k_n;
	}
	int		getN() { return key_n; }
	bool	getL() { return leaf; }
	keyType	getK(INDEX i) { return key[i]; }
	node*	getC(INDEX i) { return child[i]; }
	node*	getP() { return parent; }
	void	setN(INDEX n) { key_n = n; }
	void	setL(bool b) { leaf = b; }
	void	setK(INDEX i, INDEX n) { key[i] = n; }
	void	setC(INDEX i, node* t) { child[i] = t; }
	void	setP(node* t) { parent = t; }
	void	show() {
		cout << key_n << ": ";
		for (int i = 0; i < key_n; i++)
			cout << key[i] << " ";
	}
};

template<typename keyType>
ofstream& operator<<(ofstream& os, node<keyType>* a) {
	//cout << "In output node...\n";
	os << "0x" << a << " ";
	if (a) {
		int i, n = a->key_n;
		for (i = 0; i < n; i++)
			os << a->key[i] << " ";
	}
	//cout << "Out output node!!!\n";
	return os;
}

#endif