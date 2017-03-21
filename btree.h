#pragma once
#ifndef BTREE_H
#define BTREE_H
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <stack>
#include <set>

using namespace std;

#define DEGREE 4

template<typename keyType>
class btree {
private:
	bool leaf;
	int key_n;
	keyType key[2 * DEGREE - 1];
	btree* child[2 * DEGREE];
public:
	btree();
	void setRoot();
	bool search(keyType k);
	void split(btree* x, int i);
	void insertNon(btree* x, keyType k);
	void insert(btree*& root, keyType k);
	void del(btree* x, keyType key);
	int getN();
	bool getL();
	keyType getK(int i);
	btree* getC(int i);
	void setN(int n);
	void setL(bool b);
	void setK(int i, int n);
	void setC(int i, btree* t);
	void show();
	void clear();
};
#endif