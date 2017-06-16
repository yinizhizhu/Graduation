#ifndef STEP_TYPE_H
#define STEP_TYPE_H
#pragma once
#include <iostream>
#include <string>
using namespace std;

enum STEP_TYPE {	//enumerate the type of each operation
	FIN_STEP,	//find
	INS_STEP,	//insert
	DEL_STEP,	//delete
	UPD_STEP,	//update
	HED_STEP,	//head tag
	SPL_STEP,	//split
	MER_STEP	//merge
};
//
//string sTs(STEP_TYPE a) {
//	string ans = "mer";
//	if (a == FIN_STEP) ans = "fin";
//	else if (a == INS_STEP) ans = "ins";
//	else if (a == DEL_STEP) ans = "del";
//	else if (a == UPD_STEP) ans = "upd";
//	else if (a == HED_STEP) ans = "hed";
//	return ans;
//}

#endif