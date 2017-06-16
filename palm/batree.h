#ifndef BATREE_H
#define BATREE_H
#pragma once
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
#include "clk.h"
#include "info.h"
#include "query.h"
#include "modify.h"

using namespace std;

//#define SINGLE
//#define SHOW

#define	TEST_NUM	99
#define	EACH_NUM	9
#define	THREAD_NUM	4

#define TREE_FILE_NAME				"bplus.txt"
#define	SYNCHRONIZING_MUTEX_NAME	"__PALM_MUTEX__"
#define	QUERY_FILE_NAME				"query.txt"
#define	QRESULT_FILE_NAME			"queryResult.txt"
#define	INFO_FILE_NAME				"info.txt"
#define	IRESULT_FILE_NAME			"infoResult.txt"

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
	typedef struct node<keyType>	NODE;
	typedef struct node<keyType>*	PNODE;
	PNODE	root;
	int		deep;

	typedef struct query<keyType>	QUERY;
	vector< vector<QUERY> > queries;	//store the queries

	typedef struct info<keyType>	INFO;
	typedef struct info<keyType>*	PINFO;
	//	< PNODE, PINFO >
	unordered_map<void*, void*>		list;	//store the info for the cur to the upper node
	typedef unordered_map<void*, void*>::iterator infoIter;

	typedef struct modify<keyType>	MODIFY;
	typedef struct modify<keyType>*	PMODIFY;
	//	< PNODE parent, PMODIFY child>
	mutex							protectList;	//protect the modifyList
	unordered_map<void*, void*>		modifyList;

	vector<thread>		threads;	//store the threads
	vector<thread::id>	threadsId;

	mutex				protectOrp;
	vector<keyType>		orphanedKey;	//collect the key which is left when their node is removed

	my_clock			clk;
	double				costT;
	int					variable, testV;
	char				treeFileName[30];
	int					current;
public:
	batree(int v);
	~batree();
	void	getTree();
	void	fastRandom();				//get the query randomly
	void	outputQuery(char*	fileName);	//output the query into file
	void	outputInfo(char*	fileName);	//output the info into file
	void	palm(char* infoFileName);					//palm operation for this BPlus tree
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