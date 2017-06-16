#ifndef QUERY_H
#define QUERY_H
#pragma once
#include "stepType.h"

#include <iostream>
using namespace std;

template<typename keyType>
struct query {
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
};

#endif