#pragma once
#ifndef BATREE_H
#define BATREE_H
#include <time.h>
#include <vector>
#include <thread>
#include <random>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;

#define	DEGREEA	3

#define	TEST_NUM	99
#define	EACH_NUM	9
#define	THREAD_NUM	4

#define	SYNCHRONIZING_MUTEX_NAME "__PALM_MUTEX__"
#define	QUERY_FILE_NAME	"query.txt"
#define RESULT_FILE_NAME "queryResult.txt"

//#define NULL_STEP	0x0000
//#define	ADD_STEP	0x0001
//#define	DEL_STEP	0x0002
//#define	FIND_STEP	0x0004
//#define	SPLIT_STEP	0x0008
//#define	MERGE_STEP	0x0010
//typedef	unsigned int	STEP_TYPE;

enum STEP_TYPE {	//enumerate the type of each operation
	FIN_STEP,	//find
	INS_STEP,	//insert
	DEL_STEP,	//delete
	SPL_STEP,	//split
	MER_STEP	//merge
};

template<typename keyType>
class batree {
private:
	typedef struct node {
		bool	leaf;					//true while current node is leaf node, false for inner node
		int		key_n;					//the number of the key
		keyType	key[2 * DEGREEA - 1];	//store the key
		node*	parent;
		node*	child[2 * DEGREEA];		//store the pointer of child
		node() {
			parent = NULL;
			for (int i = 2 * DEGREEA - 1; i >= 0; i--)
				child[i] = NULL;
			leaf = false;
			key_n = 0;
		}
		int		getN() { return key_n; }
		bool	getL() { return leaf; }
		keyType	getK(int i) { return key[i]; }
		node*	getC(int i) { return child[i]; }
		node*	getP() { return parent; }
		void	setN(int n) { key_n = n; }
		void	setL(bool b) { leaf = b; }
		void	setK(int i, int n) { key[i] = n; }
		void	setC(int i, node* t) { child[i] = t; }
		void	setP(node* t) { parent = t; }
	} NODE, *PNODE;
	PNODE root;

	typedef struct query {
		STEP_TYPE	type;	//find, add, del
		keyType		key;
		PNODE		ans;	//store the result of searching
		query(STEP_TYPE t, keyType k) : type(t), key(k), ans(NULL) {}
		void setA(PNODE a) { ans = a; }
	} QUERY, *PQUERY;
	friend ofstream& operator<<(ofstream& os, const QUERY& a) {
		os << a.type << '\t' << a.key << '\t' << a.ans;
		if (a.ans) {
			os << ": ";
			int i, n = a.ans->getN();
			for (i = 0; i < n; i++)
				os << a.ans->getK(i) << " ";
		}
		return os;
	}
	vector< vector<QUERY> > queries;	//store the queries

	typedef struct info {
		STEP_TYPE	type;	//add, split, del, merge
		keyType		key;
		PNODE		cur;
		info(STEP_TYPE t, keyType k, PNODE c) {
			type = t;
			k = k;
			c = c;
		}
	} INFO, *PINFO;
	vector< vector<info> > list;	//store the info for the cur to the upper node

	vector<bool> res;
	vector<thread> threads;	//store the threads
public:
	batree();
	~batree();
	batree(batree const&) = delete;
	batree& operator=(batree const&) = delete;
	void fastRandom();				//get the query randomly
	void outputQuery(char* fileName);	//output the query in file
	void palm();					//palm operation for this BPlus tree
	void modifyNode();				//the supporting funciton
	void sync();					//the supporting function
	void fin(int p);				//testing for finding
	void* find(keyType k);								//get the node pointer
	bool search(keyType k, int p = 0);					//search k in root
	void split(PNODE x, int i);							//split the child whose index is i of node x
	void insertNon(PNODE x, keyType k);					//insert the k into the subtree whose root is node x
	void insert(keyType k);								//insert the k into root
	void merge(PNODE x, int i, PNODE y, PNODE z);		//merge node y, key i and node z, x is the parent of y and z
	void del(keyType k);								//delete the k from root
	void delNon(PNODE x, keyType k);					//delete the k from the subtree whose root is node x
	void delSet(keyType k, keyType v);					//revalue the index while the head is changed
	void shiftRTL(PNODE x, int i, PNODE y, PNODE z);	//x's right child y borrows a key and a child from x's left child of z
	void shiftLTR(PNODE x, int i, PNODE y, PNODE z);	//...
	void doShow(PNODE root, int d);
	void show();										//API for showing the btrees
	void test(keyType n);			//test the parent
	void doClear(PNODE root);
	void clear();										//API for free the sources we apply
};
#endif