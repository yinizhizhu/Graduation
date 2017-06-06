#ifndef INFO_H
#define INFO_H
#pragma once
#include "stepType.h"

#include <iostream>
using namespace std;

template<typename keyType>
struct info {	//for the leaf node
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
};

#endif