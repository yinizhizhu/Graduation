#pragma once
#ifndef BATREE_H
#define BATREE_H
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stack>
#include <mutex>
#include <vector>
#include <thread>
#include <random>
#include <fstream>
#include <utility>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <condition_variable>

using namespace std;

#define DEGREE		3
#define MIN_DEGREE	DEGREE - 1
#define MAX_DEGREE	2 * DEGREE - 1

#define	TEST_NUM	99
#define	EACH_NUM	9
#define	THREAD_NUM	4

#define TREE_FILE_NAME				"bplus.txt"
#define	SYNCHRONIZING_MUTEX_NAME	"__PALM_MUTEX__"
#define	QUERY_FILE_NAME				"query.txt"
#define	QRESULT_FILE_NAME			"queryResult.txt"
#define	INFO_FILE_NAME				"info.txt"
#define	IRESULT_FILE_NAME			"infoResult.txt"
typedef unsigned int INDEX;

template<typename keyType>
class batree {
private:
	enum STEP_TYPE {	//enumerate the type of each operation
		FIN_STEP,	//find
		INS_STEP,	//insert
		DEL_STEP,	//delete
		UPD_STEP,	//update
		SPL_STEP,	//split
		MER_STEP	//merge
	};
	friend ofstream& operator<<(ofstream& os, const STEP_TYPE& a) {
		if (a == FIN_STEP) os << "fin";
		else if (a == INS_STEP) os << "ins";
		else if (a == DEL_STEP) os << "del";
		else if (a == UPD_STEP) os << "upd";
		else os << "mer";
		return os;
	}

	typedef struct node {
		bool	leaf;					//true while current node is leaf node, false for inner node
		int		key_n;					//the number of the key
		mutex	curLock;				//lock the current node
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
		int		getI() {
			if (parent) {
				node** pc = parent->child;
				int i = 0;
				for (;; i++) {
					if (pc[i] == this)
						return i;
				}
			}
			return 0;
		}
		bool	getL() { return leaf; }
		keyType	getK(int i) { return key[i]; }
		node*	getC(int i) { return child[i]; }
		node*	getP() { return parent; }
		void	setN(int n) { key_n = n; }
		void	setL(bool b) { leaf = b; }
		void	setK(int i, int n) { key[i] = n; }
		void	setC(int i, node* t) { child[i] = t; }
		void	setP(node* t) { parent = t; }
		void	show() {
			for (int i = 0; i < key_n; i++)
				cout << key[i] << " ";
		}
	} NODE, *PNODE;
	friend ofstream& operator<<(ofstream& os, const PNODE& a) {
		os << "0x" << a << " ";
		if (a) {
			int i, n = a->key_n;
			for (i = 0; i < n; i++)
				os << a->key[i] << " ";
		}
		return os;
	}
	PNODE root;

	typedef struct query {
		STEP_TYPE	type;	//find, add, del
		keyType		key;
		bool		ans;	//store the result of searching
		query(STEP_TYPE t, keyType k) : type(t), key(k), ans(false) {}
		keyType		getK() { return key; }
		void		setA(bool a) { ans = a; }
	} QUERY, *PQUERY;
	friend ofstream& operator<<(ofstream& os, const QUERY& a) {
		//if (a.type == INS_STEP)
		//	os << "ins\t";
		//else if (a.type == DEL_STEP)
		//	os << "del\t";
		//else os << "fin\t";
		os << a.type << "\t" << a.key << "\t";
		if (a.type == FIN_STEP) {
			if (a.ans) os << "true";
			else os << "false";
		}
		return os;
	}
	vector< vector<QUERY> >		queries;	//store the queries

	vector<thread>				threads;	//store the threads
	//typedef	pair<PNODE, int>	site;
	stack<PNODE> path;
public:
	batree();
	~batree();
	void	getTree();
	void	fastRandom();
	void	outputQuery(char*	fileName);	//output the query into file
	void*	findLeaf(keyType	k);
	void	search(int	x, int	y);								//search k in root
	void	split(PNODE	x, int	i);							//split the child whose index is i of node x
	void	splitPath(PNODE	r);
	void	insert(int	x, int	y);								//insert the k into root
	void	merge(PNODE	x, int	i, PNODE	y, PNODE	z);		//merge node y, key i and node z, x is the parent of y and z
	void	del(int	x, int	y);								//delete the k from root
	void	delNon(PNODE	x, keyType	k);					//delete the k from the subtree whose root is node x
	void	delSet(keyType	k, keyType	v);					//revalue the index while the head is changed
	void	shiftRTL(PNODE	x, int	i, PNODE	y, PNODE	z);	//x's right child y borrows a key and a child from x's left child of z
	void	shiftLTR(PNODE	x, int	i, PNODE	y, PNODE	z);	//...
	void	doShow(ofstream&	out, PNODE	root, int	d);
	void	show();										//API for showing the btrees
	void	show(int	x, int	y);										//API for showing the btrees
	void	testParent(keyType	n);			//test the parent
	void	doClear(PNODE root);
	void	clear();										//API for free the sources we apply
};
#endif
