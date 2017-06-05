#pragma once
#ifndef BATREE_H
#define BATREE_H
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <mutex>
#include <vector>
#include <thread>
#include <random>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <functional>
#include <unordered_map>

using namespace std;

//#define SINGLE
//#define SHOW

#define	DEGREE	3
#define MIN_DEGREE DEGREE-1
#define MAX_DEGREE 2*DEGREE-1

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

//#define	NULL_STEP	0x0000
//#define	ADD_STEP	0x0001
//#define	DEL_STEP	0x0002
//#define	FIND_STEP	0x0004
//#define	SPLIT_STEP	0x0008
//#define	MERGE_STEP	0x0010
//typedef	unsigned INDEX	STEP_TYPE;

template<typename keyType>
class batree {
private:
	enum STEP_TYPE {	//enumerate the type of each operation
		FIN_STEP,	//find
		INS_STEP,	//insert
		DEL_STEP,	//delete
		UPD_STEP,	//update
		HED_STEP,	//head tag
		SPL_STEP,	//split
		MER_STEP	//merge
	};
	friend ofstream& operator<<(ofstream& os, const STEP_TYPE& a) {
		if (a == FIN_STEP) os << "fin";
		else if (a == INS_STEP) os << "ins";
		else if (a == DEL_STEP) os << "del";
		else if (a == UPD_STEP) os << "upd";
		else if (a == HED_STEP) os << "hed";
		else os << "mer";
		return os;
	}

	typedef struct node {
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
			for (int i = 0; i < key_n; i++)
				cout << key[i] << " ";
		}
	} NODE, *PNODE;
	friend ofstream& operator<<(ofstream& os, const PNODE& a) {
		os << "0x" << a << " ";
		if (a) {
			INDEX i, n = a->key_n;
			for (i = 0; i < n; i++)
				os << a->key[i] << " ";
		}
		return os;
	}
	PNODE	root;
	int		deep;

	typedef struct query {
		STEP_TYPE	type;	//find, add, del
		keyType		key;
		bool		ans;	//store the result of searching
		query(STEP_TYPE t, keyType k) : type(t), key(k), ans(false) {}
		void		setA(bool a) { ans = a; }
		keyType		getK() { return key; }
		void		show() {
			if (type == INS_STEP)
				cout << "ins\t";
			else if (type == DEL_STEP)
				cout << "del\t";
			else cout << "fin\t";
			cout << key << ":";
		}
	} QUERY, *PQUERY;
	friend ofstream& operator<<(ofstream& os, const QUERY& a) {
		//if (a.type == INS_STEP)
		//	os << "ins\t";
		//else if (a.type == DEL_STEP)
		//	os << "del\t";
		//else os << "fin\t";
		os << a.type << "\t" << a.key << "\t";
		if (a.ans)
			os << "Y";
		else
			os << "-";
		return os;
	}
	vector< vector<QUERY> > queries;	//store the queries

	typedef struct info {	//for the leaf node
		STEP_TYPE	type;	//add, split, del, merge
		keyType		key;
		info*		next;
		info(STEP_TYPE t, keyType k, info* n = NULL) : type(t), key(k), next(n) {}
		void		setN(info* n) { next = n; }
		STEP_TYPE	getT() { return type; }
		keyType		getK() { return key; }
		info*		getN() { return next; }
		void		show() {
			info* move = this;
			while (move) {
				cout << '\t';
				if (move->type == INS_STEP)
					cout << "INS ";
				else
					cout << "DEL ";
				cout << move->key << '\n';
				move = move->next;
			}
			cout << '\n';
		}
	} INFO, *PINFO;
	friend ofstream& operator<<(ofstream& os, const PINFO& a) {
		PINFO move = a;
		while (move) {
			os << '\t';
			//if (move->type == INS_STEP)
			//	os << "ins ";
			//else if (move->type == DEL_STEP)
			//	os << "del ";
			//else os << "fin ";
			os << move->type << " ";
			os << move->key << '\n';
			move = move->next;
		}
		os << '\n';
		return os;
	}
	//	< PNODE, PINFO >
	unordered_map<void*, void*>	list;	//store the info for the cur to the upper node
	typedef unordered_map<void*, void*>::iterator infoIter;

	typedef struct modify {
		STEP_TYPE	type;
		keyType		key, old;
		PNODE		leaf;
		modify*		next;
		modify(STEP_TYPE t, keyType k, PNODE l, modify* n = NULL) : 
			type(t), key(k), leaf(l), next(n) {}
		void		setT(STEP_TYPE s) { type = s; }
		void		setK(keyType k) { key = k; }
		void		setL(PNODE l) { leaf = l; }
		void		setN(modify* n) { next = n; }
		void		setO(keyType o) { old = o; }
		STEP_TYPE	getT() { return type; }
		keyType		getK() { return key; }
		PNODE		getL() { return leaf; }
		modify*		getN() { return next; }
		keyType		getO() { return old; }
		void		show() {
			modify* move = this;
			while (move) {
				cout << '\t';
				if (move->type == INS_STEP)
					cout << "INS ";
				else if (move->type == DEL_STEP)
					cout << "DEL ";
				else if (move->type == UPD_STEP)
					cout << "UPD ";
				else if (move->type == HED_STEP)
					cout << "HED ";
				cout << move->key << '\n';
				move = move->next;
			}
			cout << '\n';
		}
	} MODIFY, *PMODIFY;
	friend ofstream& operator<<(ofstream& os, const PMODIFY& a) {
		PMODIFY move = a;
		os << "0x" << a << ":\n";
		while (move) {
			os << "\t";
			os << move->type << " ";
			if (move->type == UPD_STEP)
				os << move->old << "->";
			os << move->key << ": ";
			os << move->leaf << '\n';
			move = move->next;
		}
		os << '\n';
		return os;
	}
	//	< PNODE parent, PMODIFY child>
	mutex						protectList;	//protect the modifyList
	unordered_map<void*, void*> modifyList;

	vector<thread>		threads;	//store the threads
	vector<thread::id>	threadsId;

	mutex				protectOrp;
	vector<keyType>		orphanedKey;	//collect the key which is left when their node is removed
public:
	batree();
	~batree();
	batree(batree const&) = delete;
	batree& operator=(batree const&) = delete;
	void	getTree();
	void	fastRandom();				//get the query randomly
	void	outputQuery(char*	fileName);	//output the query into file
	void	outputInfo(char*	fileName);	//output the info into file
	void	palm();					//palm operation for this BPlus tree
	//int		getDeep();										//support the palm
	void	handleRoot(infoIter	ope);									//support the palm
	void	modifyNode(infoIter	inf, INDEX	p);				//the supporting funciton: p ( 0 - leaf, 1 - inner)
	int		inParent(PNODE	key, PNODE	parent);
	int		inBuffer(vector<keyType>&	buffer, keyType	key);	//support the modifynode
	void	getBuffer(vector<PNODE>&	child, infoIter	inf,
		vector<keyType>&	buffer, INDEX	p);	//suppor the modifyNode
	void	soft(vector<PNODE>&	child, vector<keyType>&	buffer);
	void	collect(PNODE head);
	void	showOrp();
	int		check(int	i, vector<PNODE>&	child);
	void	testMS();
	void	merge(keyType	key, PNODE	y, PNODE	z);		//merge node y, key i and node z, x is the parent of y and z
	void	shiftRTL(keyType&	key, int	n, PNODE	y, PNODE	z);	//x's right child y borrows a key and a child from x's left child of z
	void	shiftLTR(keyType&	key, int	n, PNODE	y, PNODE	z);	//...
	void	pushModify(PNODE	parent, PMODIFY	child, PMODIFY	moveChild);	//support the modifynode
	void	outputModify(PNODE	parent, PMODIFY	child);		//Just for testing
	void	swap(keyType&	a, keyType&	b);					//support the modifynode
	void	showChildB(vector<PNODE>&	childBuf);									//test the childBuf in modifynode
	void	showBuffer(vector<keyType>&	buffer, vector<PNODE>& child);			//test the buffer in modifynode
	void	showModifyList(char*	fileName);
	void	find(INDEX	p);				//testing for finding
	void*	findLeaf(keyType	k);							//get the leaf node pointer
	bool	search(keyType	k);								//search k in root
	void	doShow(ofstream&	out, PNODE	root, int	d);
	void	show(int	tag = -1, keyType	key = 0);										//API for showing the btrees
	void	testParent(keyType	n);			//test the parent
	void	doClear(PNODE	root);
	void	clear();										//API for free the sources we apply
};
#endif