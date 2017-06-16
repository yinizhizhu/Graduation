#ifndef MODIFY_H
#define MODIFY_H
#pragma once
#include "node.h"
#include "stepType.h"

#include <iostream>
using namespace std;

template<typename keyType>
struct modify {
	STEP_TYPE	type;
	keyType		key, old;
	void*		leaf;
	modify*		next;
	modify(STEP_TYPE t, keyType k, void* l, modify* n = NULL) :
		type(t), key(k), leaf(l), next(n) {}
	void		setT(STEP_TYPE s) { type = s; }
	void		setK(keyType k) { key = k; }
	void		setL(void* l) { leaf = l; }
	void		setN(modify* n) { next = n; }
	void		setO(keyType o) { old = o; }
	STEP_TYPE	getT() { return type; }
	keyType		getK() { return key; }
	void*		getL() { return leaf; }
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
};

template<typename keyType>
ofstream& operator<<(ofstream& os, modify<keyType>* a) {
	//cout << "In output modify...\n";
	modify<keyType>* move = a;
	os << "0x" << a << ":\n";
	while (move) {
		os << "\t";
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
		if (move->type == UPD_STEP)
			os << move->old << "->";
		os << move->key << ": ";
		os << (node<keyType>*)move->leaf << '\n';
		move = move->next;
	}
	os << '\n';
	//cout << "Out output modify!!!\n";
	return os;
}
#endif