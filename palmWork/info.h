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
			else if (move->type == DEL_STEP)
				cout << "DEL ";
			else if (move->type == UPD_STEP)
				cout << "UPD ";
			else if (move->tyep == HED_STEP)
				cout << "HED ";
			else
				cout << "MER ";
			cout << move->key << '\n';
			move = move->next;
		}
		cout << '\n';
	}
};

template<typename keyType>
ofstream& operator<<(ofstream& os, info<keyType>* a) {
	cout << "In output info...\n";
	info<keyType>* move = a;
	while (move) {
		os << '\t';
		if (move->type == INS_STEP)
			os << "ins ";
		else if (move->type == DEL_STEP)
			os << "del ";
		else if (move->type == UPD_STEP)
			os << "upd ";
		else if (move->type == HED_STEP)
			os << "hed ";
		else
			os << "mer ";
		os << move->key << '\n';
		move = move->next;
	}
	os << '\n';
	cout << "Out output info!!!\n";
	return os;
}
#endif