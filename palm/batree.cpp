#include "batree.h"

template<typename keyType>
batree<keyType>::batree(int v) {	//initial
	root = new NODE();
	deep = 1;
	if (!root)
		cout << "The room is not enough!!!!!\n";
	root->setL(true);	//start with leaf
	queries.resize(THREAD_NUM);
	threadsId.resize(THREAD_NUM);

	testV = v;
	variable = 20 * v*THREAD_NUM;
	sprintf(treeFileName, "data\\data%d\\bPlus%d.txt", v, variable);
	ofstream out(treeFileName);
	out.close();

	costT = 0;
}

template<typename keyType>
batree<keyType>::~batree() {	//free the sources
	cout << "In clean\n";
	clear();

	//cout << "Mid clean after root\n";

	for (int i = 0; i < THREAD_NUM; i++)//clean queries
		queries[i].clear();
	queries.clear();

	//cout << "Mid clean after queries\n";

	infoIter move = list.begin();	//clean list
	while (move != list.end()) {
		PINFO fuck = (PINFO)move->second, tmp;
		while (fuck) {
			tmp = fuck->getN();
			delete fuck;
			fuck = tmp;
		}
		move++;
	}
	list.clear();

	//cout << "Mid clean after list\n";

	move = modifyList.begin();	//clean list
	while (move != modifyList.end()) {
		PMODIFY fuck = (PMODIFY)move->second, tmp;
		while (fuck) {
			tmp = fuck->getN();
			delete fuck;
			fuck = tmp;
		}
		move++;
	}
	modifyList.clear();

	//cout << "Mid clean after modifyList\n";

	threads.clear();
	threadsId.clear();
	orphanedKey.clear();
	cout << "Out clean!!!\n";
}

template<typename keyType>
void batree<keyType>::getTree() {
	PNODE move = root;
	move->setL(false);
	move->setN(3);
	move->setK(0, 6);
	move->setK(1, 12);
	move->setK(2, 18);
	for (int i = 0, j = 2, k = 0; i <= 3; i++, j += 6) {
		PNODE tmp = new NODE(move, 2, false);
		tmp->setK(0, j);
		tmp->setK(1, j + 2);
		for (int p = 0; p < 3; p++) {
			int n = 2;
			if (k == 22) n = 5;
			PNODE one = new NODE(tmp, n, true);
			for (int m = 0; m < n; m++)
				one->setK(m, k++);
			tmp->setC(p, one);
		}
		move->setC(i, tmp);
	}
	deep = 3;
	show();
}

template<typename keyType>
void batree<keyType>::fastRandom() {	//get the query randomly
	cout << "In fastRandom\n";
#ifndef SINGLE
	getTree();
	//get the search query randomly, stored in 'queries'
	for (int i = 0; i < THREAD_NUM; i++)
		for (int j = 0; j < EACH_NUM; j++)
			queries[i].push_back(QUERY(FIN_STEP, rand() % 99));
	outputQuery(QUERY_FILE_NAME);

	clk.start();
	for (int i = 0; i < THREAD_NUM; i++)
		threads.push_back(thread(&batree<keyType>::find, this, i));
	//sync
	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
	threads.clear();
	costT += clk.stop();

	//cout << "finish the search!\n";
	outputQuery(QRESULT_FILE_NAME);
	//get the Delete&Insert query randomly, store in 'list'
	current = 0;
	PNODE p;
	keyType k;
	infoIter item;
	for (int i = 0; i < variable; i++) {
		k = rand() % 99;	//while keytype is INT
		p = (PNODE)findLeaf(k);
		item = list.find(p);
		if (item != list.end()) {
			PINFO head = (PINFO)item->second, tmp;
			tmp = new INFO((STEP_TYPE)(rand() % 2 + 1), k);
			tmp->setN(head);
			item->second = tmp;
		}
		else {
			if (list.size() == 4) {
				char infoFileName[30];
				sprintf(infoFileName, "data\\data%d\\info%d.txt", testV, current++);
				palm(infoFileName);
				show();
			}
			list[p] = new INFO((STEP_TYPE)(rand() % 2 + 1), k);
		}
	}
	if (list.size()) {
		char infoFileName[30];
		sprintf(infoFileName, "data\\data%d\\info%d.txt", testV, current);
		palm(infoFileName);
		show();
	}
	ofstream out("data.txt", ios::app);
	out << variable << "\tYour program executed time is " << costT << "ms.\n";
	out.close();
	outputQuery(QRESULT_FILE_NAME);
#else
	int i, j;
	for (i = 0; i < THREAD_NUM; i++)
		for (j = 0; j < EACH_NUM; j++) {
			PNODE p;
			keyType k = rand() % 1000;
			p = (PNODE)findLeaf(k);

			if (deep > 1) {
				cout << "List is:\n";
				list[p] = new INFO(INS_STEP, k);
				for (infoIter item = list.begin(); item != list.end(); item++)
					((PINFO)(item->second))->show();
			}
			else {
				cout << "modifyList is:\n";
				modifyList[p] = new MODIFY(INS_STEP, k, NULL);
				for (infoIter item = modifyList.begin(); item != modifyList.end(); item++)
					((PMODIFY)(item->second))->show();
			}

			palm();
			show(0, k);

			if (deep > 1) {
				cout << "After palm! List is:\n";
				for (infoIter item = list.begin(); item != list.end(); item++)
					((PINFO)(item->second))->show();
			}
			else {
				cout << "After palm! modifyList is:\n";
				for (infoIter item = modifyList.begin(); item != modifyList.end(); item++)
					((PMODIFY)(item->second))->show();
			}
		}

	int n;
	cout << "Input the number of test cases: ";
	//queries[0][0].key = 478;
	//search(0, 0);
	//del(0, 0);
	cin >> n;
	for (; n > 0; n--) {
		PNODE p;
		cout << endl << "\tInput the number: ";
		cin >> i;
		p = (PNODE)findLeaf(i);

		if (deep > 1) {
			cout << "List is:\n";
			list[p] = new INFO(DEL_STEP, i);
			for (infoIter item = list.begin(); item != list.end(); item++)
				((PINFO)(item->second))->show();
		}
		else {
			cout << "modifyList is:\n";
			modifyList[p] = new MODIFY(DEL_STEP, i, NULL);
			for (infoIter item = modifyList.begin(); item != modifyList.end(); item++)
				((PMODIFY)(item->second))->show();
		}

		palm();
		show(1, i);

		if (deep > 1) {
			cout << "After palm! List is:\n";
			for (infoIter item = list.begin(); item != list.end(); item++)
				((PINFO)(item->second))->show();
		}
		else {
			cout << "After palm! modifyList is:\n";
			for (infoIter item = modifyList.begin(); item != modifyList.end(); item++)
				((PMODIFY)(item->second))->show();
		}
	}
#endif
	cout << "Out fastRandom!!!\n";
}

template<typename keyType>
void batree<keyType>::outputQuery(char* fileName) {	//output the query
	ofstream out(fileName);
	for (int i = 0; i < THREAD_NUM; i++) {
		out << i << "\t-\t" << threadsId[i] << ":\n";
		for (int j = 0; j < EACH_NUM; j++) {
			out << "fin\t" << queries[i][j].key << "\t";
			if (queries[i][j].ans)
				out << "Y";
			else
				out << "-";
			out << '\n';
		}
		out << "\n";
	}
	out.close();
}

template<typename keyType>
void batree<keyType>::outputInfo(char* fileName) {	//output the info
	ofstream out(fileName);
	infoIter tmp = list.begin();
	for (; tmp != list.end(); tmp++) {
		out << (PNODE)tmp->first << ": \n";
		PINFO move = (PINFO)tmp->second;
		while (move) {
			out << '\t';
			if (move->type == INS_STEP)
				out << "ins ";
			else
				out << "del ";
			out << move->key << '\n';
			move = move->next;
		}
		out << '\n';
	}
	out.close();
}

template<typename keyType>
void batree<keyType>::palm(char* infoFileName) {	//palm operation for this BPlus tree
	//stage 1: search
	//cout << "In palm\n";

	outputInfo(infoFileName);
	
	//cout << "\nStarting modify-leaf-node...\n";

	//stage 2: modify-leaf-NODE	
	int i = 0, counter;
	infoIter move= list.begin();
	counter = list.size();

	//cout << "Start timing...\n";
	clk.start();
	for (i = 0; i < counter; i++, move++)
		threads.push_back(thread(&batree<keyType>::modifyNode, this, move, 0));
		//sync
	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
	threads.clear();
	costT += clk.stop();
	//cout << "End timing!!!\n";

	for (i = 0; i < counter; i++)	//remove the info which is handled
		list.erase(list.begin());
#ifdef SINGLE
	showModifyList("inner.txt");
#endif
	//cout << "finish the modify-leaf-node!\n\nStarting modify-inner-node...\n";
	//outputInfo(IRESULT_FILE_NAME);

	//stage 3: modify-inner-NODE
	//cout << "Deep: " << deep << '\n';
	//cout << "Start timing...\n";
	clk.start();
	for (int j = 2; j < deep; j++) {
		//cout << "\tModifying inner node...\n";
		counter = modifyList.size();
		for (i = 0, move = modifyList.begin(); i < counter; i++, move++) {
			//cout << "\tPush " << ((PMODIFY)move->second)->getT() << "\n";
			//if (((PMODIFY)move->second)->getT() < 0) {
			//	int b;
			//	cout << "\tInput the b: ";
			//	cin >> b;
			//}
			threads.push_back(thread(&batree<keyType>::modifyNode, this, move, 1));
		}
		//sync
		for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
		threads.clear();

		for (i = 0; i < counter; i++)	//remove the info which is handled
			modifyList.erase(modifyList.begin());
	}
	costT += clk.stop();
	//cout << "End timing!!!\n";

#ifdef SINGLE
	showModifyList("root.txt");
#endif
	//cout << "finish the modify-inner-node!\n\nStarting modify-root-node...\n";

	//stage 4: handle the root
	//cout << "Start timing...\n";
	clk.start();
	move = modifyList.begin();
	//cout << modifyList.size() << endl;
	while (move != modifyList.end()) {
		//cout << "\tModifying root...\n";
		//cout << "\tPush " << ((PMODIFY)move->second)->getT() << "\n";
		//if (((PMODIFY)move->second)->getT() < 0) {
		//	int a;
		//	cout << "\tInput the a: ";
		//	cin >> a;
		//}
		thread(&batree<keyType>::handleRoot, this, move).join();
		modifyList.erase(move);
		move = modifyList.begin();
	}
	costT += clk.stop();
	//cout << "End timing!!!\n";

	//cout << "finish modify-root-node...\n";
	showOrp();
	//cout << "Out palm!!!\n";
}

template<typename keyType>
void batree<keyType>::handleRoot(infoIter inf) {	//the supporting funciton
	//cout << "In handleRoot\n";
	vector<PNODE>	childBuf;
	vector<keyType>	buffer;
	PNODE cur = root;
	int i, p = deep - 1, n = cur->getN();//just for int
	//cout << "p: " << p << endl;
	for (i = 0; i < n; i++)
		buffer.push_back(cur->getK(i));
#ifdef SHOW
	//if (p)
	//	outputModify(PNODE(inf->first), (PMODIFY)inf->second);
#endif
	getBuffer(childBuf, inf, buffer, p);
	n = buffer.size();
	sort(buffer.begin(), buffer.end());//sorting 
#ifdef SHOW
	//showBuffer(buffer, childBuf);	//test
#endif
	//showChildB(childBuf);		//test

	int j = 0, k = 0;
	if (n > MAX_DEGREE) {	//	NEED AHNDLE THE DIFFER 
		//cout << "MAX_D1\n";
		//big - split();
		PMODIFY child = NULL;
		PNODE parent = new NODE(NULL, 1, false), buf;
		root->setP(parent);
		root = parent;	//new root: deeper
		deep++;
		parent->setC(0, cur);

		cur->setN(MIN_DEGREE);//set the number of key
		if (cur->getL())
			child = new MODIFY(UPD_STEP, buffer[j], cur, NULL);
		else
			child = new MODIFY(HED_STEP, buffer[j], cur, NULL);
		PMODIFY moveChild = child;
		while (n > MAX_DEGREE) {
			buf = (PNODE)moveChild->getL();
			for (i = 0; i < MIN_DEGREE; i++, j++) {
				buf->setK(i, buffer[j]);
				if (p) {
					childBuf[k]->setP(buf);
					buf->setC(i, childBuf[k++]);
				}
			}
			if (p) {
				childBuf[k]->setP(buf);
				buf->setC(i, childBuf[k++]);
			}
			moveChild->setN(new MODIFY(INS_STEP, buffer[j], new NODE(parent, MIN_DEGREE, cur->getL()), NULL));
			n -= MIN_DEGREE;
			if (p) {
				j++;
				n--;
			}
			moveChild = moveChild->getN();
		}
		buf = (PNODE)moveChild->getL();
		buf->setN(n);//set the number of key
		for (i = 0; n > 0; i++, j++, n--) {
			buf->setK(i, buffer[j]);
			if (p)  {
				childBuf[k]->setP(buf);
				buf->setC(i, childBuf[k++]);
			}
		}
		if (p)  {
			childBuf[k]->setP(buf);
			buf->setC(i, childBuf[k]);
		}

		PMODIFY tmp = child->getN();
		tmp->setT(UPD_STEP);
		parent->setC(1, (PNODE)tmp->getL());
		parent->setK(0, tmp->getK());
		delete child;
		child = tmp;
		if (tmp == moveChild)
			delete child;
		else
			pushModify(parent, child, moveChild);
		//cout << "MAX_D1!!!!!!!!\n";
	}
	else {
		//cout << "MAX_D2\n";
		if (n == 0) {	//lower
			if (!root->getL()) {
				root = childBuf[0];
				root->setP(NULL);
				deep--;
				delete cur;
			}
			else
				root->setN(0);
		}
		else {
			cur->setN(n);//set the number of key
			for (i = 0; n > 0; i++, j++, n--) {
				cur->setK(i, buffer[j]);
				if (p) {
					childBuf[k]->setP(cur);
					cur->setC(i, childBuf[k++]);
				}
			}
			if (p) {
				childBuf[k]->setP(cur);
				cur->setC(i, childBuf[k]);
			}
		}
		//cout << "MAX_D2!!!!!!!!\n";
	}
	//cout << "Out handleRoot!!!\n";
}

template<typename keyType>
void batree<keyType>::modifyNode(infoIter inf, INDEX p) {	//the supporting funciton
	//cout << "In modify\n";

	vector<PNODE>	childBuf;
	vector<keyType>	buffer;
	PNODE cur = (PNODE)inf->first;

	keyType key;
	if (p == 0)
		key = ((PINFO)inf->second)->getK();
	else
		key = ((PMODIFY)inf->second)->getK();

	int i, n = cur->getN();//just for int
	for (i = 0; i < n; i++)
		buffer.push_back(cur->getK(i));
#ifdef SHOW
	//if (p)
	//	outputModify(PNODE(inf->first), (PMODIFY)inf->second);
#endif
	getBuffer(childBuf, inf, buffer, p);
	sort(buffer.begin(), buffer.end());//sorting 
#ifdef SHOW
	//showBuffer(buffer, childBuf);	//test
#endif
	//showChildB(childBuf);		//test

	int j = 0, k = 0;
	n = buffer.size();
	PMODIFY	child = NULL;
	PNODE	parent = cur->getP(), buf;
	if (n > MAX_DEGREE) {	//	NEED AHNDLE THE DIFFER 
		//cout << "MAX_D1\n";
		//big - split();
		cur->setN(MIN_DEGREE);//set the number of key
		if (cur->getL() && buffer[j] != cur->getK(0)) {
			child = new MODIFY(UPD_STEP, buffer[j], cur, NULL);
			child->setO(cur->getK(0));
		}
		else
			child = new MODIFY(HED_STEP, buffer[j], cur, NULL);
		PMODIFY moveChild = child;
		while (n > MAX_DEGREE) {
			buf = (PNODE)moveChild->getL();
			for (i = 0; i < MIN_DEGREE; i++, j++) {
				buf->setK(i, buffer[j]);
				if (p) {
					childBuf[k]->setP(buf);
					buf->setC(i, childBuf[k++]);
				}
			}
			if (p)  {
				childBuf[k]->setP(buf);
				buf->setC(i, childBuf[k++]);
			}
			moveChild->setN(new MODIFY(INS_STEP, buffer[j], new NODE(parent, MIN_DEGREE, cur->getL()), NULL));
			n -= MIN_DEGREE;
			if (p) {
				j++;
				n--;
			}
			moveChild = moveChild->getN();
		}
		buf = (PNODE)moveChild->getL();
		buf->setN(n);//set the number of key
		for (i = 0; n > 0; i++, j++, n--) {
			buf->setK(i, buffer[j]);
			if (p) {
				childBuf[k]->setP(buf);
				buf->setC(i, childBuf[k++]);
			}
		}
		if (p) {
			childBuf[k]->setP(buf);
			buf->setC(i, childBuf[k]);
		}
		if (parent) {
#ifdef SHOW
			if (parent == root) {
				cout << "Operation: \n";
				child->show();
				cout << "\nEnd Operation!\n";
			}
#endif
			pushModify(parent, child, moveChild);
		}

		//cout << "MAX_D1!!!!!!!!\n";
	}
	else {
		//cout << "MAX_D2\n";
		if (n < MIN_DEGREE) {
			if (n)
				child = new MODIFY(DEL_STEP, buffer[j], cur, NULL);
			else
				child = new MODIFY(DEL_STEP, cur->getK(0), cur, NULL);
		}
		else {
			if (cur->getL() && buffer[j] != cur->getK(0))
				child = new MODIFY(UPD_STEP, buffer[j], cur, NULL);
		}
		if (child)
			child->setO(cur->getK(0));
		cur->setN(n);//set the number of key
		for (i = 0; n > 0; i++, j++, n--) {
			cur->setK(i, buffer[j]);
			if (p) {
				childBuf[k]->setP(cur);
				cur->setC(i, childBuf[k++]);
			}
		}
		if (p) {
			childBuf[k]->setP(cur);
			cur->setC(i, childBuf[k]);
		}
		if (parent && child) {
#ifdef SHOW
			if (parent == root) {
				cout << "Operation: \n";
				child->show();
				cout << "\nEnd Operation!\n";
			}
#endif
			pushModify(parent, child, child);
		}
		//cout << "MAX_D2!!!!!!!!\n";
	}
	//cout << "Out modify!!!\n";
}

template<typename keyType>
int batree<keyType>::inParent(PNODE child, PNODE parent) {	//Done
	int n = parent->getN();
	for (int i = 0; i <= n; i++)
		if (child == parent->getC(i))
			return i;
	return -1;
}

template<typename keyType>
int batree<keyType>::inBuffer(vector<keyType>& buffer, keyType key) {
	//cout << "In inBuffer\n";
	int i = 0, n = buffer.size();
	for (; i < n; i++)
		if (buffer[i] == key)
			break;
	//cout << "Out inBuffer!!!\n";
	return i;
}

template<typename keyType>
void batree<keyType>::getBuffer(vector<PNODE>& child, infoIter inf, vector<keyType>& buffer, INDEX p) {
	//cout << "In getBuffer\n";
	//cout << "p = " << p << endl;
	if (p == 0) {
		//cout << "\tLeaf!\n";
		void* ope = inf->second, *move;
		int i, len;
		keyType		key;
		STEP_TYPE	type;
		while (ope) {
			if (deep > 1) {
				key = ((PINFO)ope)->getK();
				type = ((PINFO)ope)->getT();
				move = ((PINFO)ope)->getN();
			}
			else {
				key = ((PMODIFY)ope)->getK();
				type = ((PMODIFY)ope)->getT();
				move = ((PMODIFY)ope)->getN();
			}
			i = inBuffer(buffer, key);
			len = buffer.size();
			switch (type) {
			case INS_STEP:
				if (i == len)	//push into the last position
					buffer.push_back(key);
				break;
			case DEL_STEP:
				if (i < len) {
					if (len > 1) {
						swap(buffer[i], buffer[len - 1]);
						buffer.erase(buffer.begin() + len - 1);
					}
					else if (len == 1)
						buffer.erase(buffer.begin());
				}
				break;
			default: break;
			}
			delete ope; //free the room
			ope = move;
		}
	}
	else {
		//cout << "\tInner!\n";
		PNODE cur = (PNODE)inf->first, tmp;
		PMODIFY ope = (PMODIFY)inf->second, move, head = NULL;
		int i, j = 0;
		while (ope) {
			keyType key = ope->getK();
			move = ope->getN();
			switch (ope->getT()) {
			case INS_STEP:
				buffer.push_back(key);
				child.push_back((PNODE)ope->getL());
				delete ope; //free the room
				break;
			case DEL_STEP:
				tmp = (PNODE)ope->getL();
				i = inParent(tmp, cur);
				for (; j < i; j++)
					child.push_back(cur->getC(j));	//get the children: j need to tag
				if (tmp->getN() == 0) {
					if (i > 0) {
						int len = buffer.size();
						i = inBuffer(buffer, cur->getK(i - 1));
						if (i < len) {
							swap(buffer[i], buffer[len - 1]);
							buffer.erase(buffer.begin() + len - 1);
						}
						else
							buffer.erase(buffer.begin());
					}
					else
						buffer.erase(buffer.begin());
					collect(tmp);	//collect the descendants
					j++;
				}
				else {
					child.push_back(cur->getC(j++));
					if (i == 0 && tmp->getL()) {
						head = ope; //free the room
						break;
					}
				}
				delete ope;
				break;
			case UPD_STEP:
				if (ope->getL()) {
					i = inParent((PNODE)ope->getL(), cur);
					for (; j <= i; j++)
						child.push_back(cur->getC(j));
				}
				if (key != ope->getO()) {
					i = inBuffer(buffer, ope->getO());
					if (i < (int)buffer.size())
						buffer[i] = key;
					else {
						if (cur->getP()) {
							ope->setL(NULL);
							ope->setN(NULL);
#ifdef SHOW
							if (cur->getP() == root) {
								cout << "Operation: \n";
								ope->show();
								cout << "\nEnd Operation!\n";
							}
#endif
							pushModify(cur->getP(), ope, ope);
							break;
						}
					}
				}
				delete ope; //free the room
				break;
			case HED_STEP:
				i = inParent((PNODE)ope->getL(), cur);
				for (; j <= i; j++)
					child.push_back(cur->getC(j));
				delete ope; //free the room
				break;
			default:
				//cout << "\t@Default " << ope->getT() << "\n";
				break;
			}
			ope = move;
		}
		for (; j <= cur->getN(); j++)
			child.push_back(cur->getC(j));
		soft(child, buffer);	//it is import to maintain the structure of BPlus Trees
		if (head) {
			if (head->getO() != child[0]->getK(0) && cur->getP()) {
				head->setT(UPD_STEP);
				head->setL(NULL);
				head->setN(NULL);
				head->setK(child[0]->getK(0));
#ifdef SHOW
				if (cur->getP() == root) {
					cout << "Operation: \n";
					head->show();
					cout << "\nEnd Operation!\n";
				}
#endif
				pushModify(cur->getP(), head, head);
			}
		}
		//cout << "\t*****"<< child.size() - buffer.size() << "*****\n";
	}
	//cout << "Out getBuffer!!!\n";
}

template<typename keyType>
void batree<keyType>::soft(vector<PNODE>& child, vector<keyType>& buffer) {	//BIG TROUBLE
	if (check(0, child) == -1)
		return;
	//cout << "In soft:\t" << child.size() << ", " << buffer.size() << endl;
	//if (child.size() - buffer.size() != 1) {
	//	cout << "\nBuffer: len="<<buffer.size()<<", ";
	//	for (int i = 0; i < (int)buffer.size(); i++)
	//		cout << buffer[i] << " ";
	//	cout << "\n";
	//	cout << "\nChild: len="<<child.size()<<"\n";
	//	for (int i = 0; i < (int)child.size(); i++) {
	//		child[i]->show();
	//		cout << "\n";
	//	}
	//}
	while (buffer.size() && child[0]->getN() < MIN_DEGREE) {
		if (child[0]->getN() + child[1]->getN() < MAX_DEGREE - 1) {
			//cout << "We are merging\n";
			merge(buffer[0], child[0], child[1]);
			buffer.erase(buffer.begin());
			child.erase(child.begin() + 1);
		}
		else {
			//cout << "We are borrowing\n";
			shiftRTL(buffer[0], MIN_DEGREE - child[0]->getN(), child[0], child[1]);
		}
	}
	int i = check(1, child);
	while (i > 0 && buffer.size()) {
		if (child[i - 1]->getN() + child[i]->getN() < MAX_DEGREE - 1) {
			merge(buffer[i - 1], child[i - 1], child[i]);
			buffer.erase(buffer.begin() + i - 1);
			child.erase(child.begin() + i);
		}
		else
			shiftLTR(buffer[i - 1], MIN_DEGREE - child[i]->getN(), child[i - 1], child[i]);
		i = check(i, child);
	}
	//cout << "Out soft!!!\n";
	//showChildB(child);//test for watching the content of child
}

template<typename keyType>
void batree<keyType>::collect(PNODE root) {
	//cout << "Collecting...\n";
	if (root) {
		if (root->getL()) {
			lock_guard<mutex> t(protectOrp);
			for (int i = 0; i < root->getN(); i++)
				orphanedKey.push_back(root->getK(i));
		}
		else
			for (int i = 0; i <= root->getN(); i++)
				collect(root->getC(i));
		delete root;
	}
	//cout << "Finish collection!!!\n";
}

template<typename keyType>
void batree<keyType>::showOrp() {
	int i = 0, n = orphanedKey.size();
	for (; i < n; i++)
		cout << orphanedKey[i] << " ";
	cout << endl;
}

template<typename keyType>
int batree<keyType>::check(int i, vector<PNODE>& child) {
	int n = child.size();
	for (; i < n; i++)
		if (child[i]->getN() < MIN_DEGREE)
			return i;
	return -1;
}

template<typename keyType>
void batree<keyType>::testMS() {
	cout << endl;
	cout << "\tmerge 18:\n";
	root->setN(2);
	merge(18, root->getC(2), root->getC(3));
	show();
	cout << endl;
	cout << "\tmerge 4:\n";
	PNODE tmp = root->getC(0);
	tmp->setN(1);
	merge(4, tmp->getC(1), tmp->getC(2));
	show();
	cout << endl;
	cout << tmp->key[0] << "\tRTL 3:\n";
	shiftRTL(tmp->key[0], 3, tmp->getC(0), tmp->getC(1));
	show();
	cout << endl;
	cout << tmp->key[0] << "\tLTR 3:\n";
	shiftLTR(tmp->key[0], 4, tmp->getC(0), tmp->getC(1));
	show();
	cout << endl;
	cout << root->key[1] << "\tRTL 3:\n";
	shiftRTL(root->key[1], 3, root->getC(1), root->getC(2));
	show();
	cout << endl;
	cout << root->key[1] << "\tLTR 3:\n";
	shiftLTR(root->key[1], 2, root->getC(1), root->getC(2));
	show();

	collect(root->getC(3));
	showOrp();
	root->setN(2);
	show();
}

template<typename keyType>
void batree<keyType>::merge(keyType key, PNODE y, PNODE z) {
	//cout << "In merge\n";
	//y - left node, z - right node
	int i = y->getN(), j = 0, n = z->getN();
	if (!y->getL())	//inner node
		y->setK(i++, key);
	for (; j < n; i++, j++)
		y->setK(i, z->getK(j));
	if (!y->getL()) {	//inner node
		for (i = y->getN() + 1, j = 0; j <= n; j++, i++) {
			y->setC(i, z->getC(j));
			y->getC(i)->setP(y);
		}
	}
	if (!y->getL())	//inner node
		y->setN(y->getN() + n + 1);
	else
		y->setN(y->getN() + n);
	delete z;
	//show();//test
	//cout << "Out merge!!!\n";
}

template<typename keyType>
void batree<keyType>::shiftLTR(keyType& key, int n, PNODE y, PNODE z) {
	//cout << "In shiftLTR\n";
	//z borrows n nodes from y
	int i = z->getN();
	z->setN(i + n);
	for (--i; i >= 0; i--)
		z->setK(i + n, z->getK(i));
	if (!z->getL()) {
		for (i = z->getN() - n; i >= 0; i--)
			z->setC(i + n, z->getC(i));
	}

	int k = y->getN(), j = n - 1;
	y->setN(k - n);
	i = k - 1;
	if (!y->getL()) { //inner node
		z->setK(j, key);
		z->setC(j, y->getC(k));
		z->getC(j)->setP(z);
		j--, k--;
	}
	if (!z->getL()) {
		for (int m = j; m >= 0; m--, k--){
			z->setC(m, y->getC(k));
			z->getC(m)->setP(z);
		}
	}
	for (; j >= 0; i--, j--)
		z->setK(j, y->getK(i));

	if (!y->getL()) //inner node
		key = y->getK(i);
	else
		key = z->getK(0);
	//cout << "Out shiftLTR!!!\n";
}

template<typename keyType>
void batree<keyType>::shiftRTL(keyType& key, int n, PNODE y, PNODE z) {
	//y borrows n nodes from z
	int i = y->getN(), move = 0, j = 0, k = 0;
	y->setN(i + n);
	if (!y->getL()) { //inner
		y->setK(i, key);
		y->setC(i + 1, z->getC(k));
		y->getC(i + 1)->setP(y);
		i++, k++;
		move++;
	}
	if (!y->getL()) {
		int a = i, b = move;
		for (; b < n; b++, a++, k++)  {
			y->setC(a + 1, z->getC(k));
			y->getC(a + 1)->setP(y);
		}
	}
	for (; move < n; move++, i++, j++)
		y->setK(i, z->getK(j));

	key = z->getK(j);
	if (!y->getL())
		j++;
	i = z->getN();
	z->setN(i - n);
	if (!z->getL()) {
		int a = i, b = j;
		for (; b < a; b++, k++)
			z->setC(b - n, z->getC(k));
		z->setC(b - n, z->getC(k));
	}
	for (; j < i; j++)
		z->setK(j - n, z->getK(j));
}

template<typename keyType>
void batree<keyType>::pushModify(PNODE parent, PMODIFY child, PMODIFY moveChild){
	protectList.lock();	//protect the modifyList
	//cout << "In pushModify\n";
	//outputModify(parent, child);//test;
#ifdef SHOW
	char name[66];
	sprintf(name, "data\\%d_%d.txt", current, parent);
	ofstream out(name, ios::app);
	out << "Parent: ";
	out << parent;
	out << "\n";
	out << "In Child:\n";
	out << child;
	out << "\n";
#endif
	infoIter step = modifyList.find(parent);
	if (step == modifyList.end())
		modifyList[parent] = child;
	else {
		keyType ck = child->getK();
		PMODIFY head = (PMODIFY)step->second;
		if (ck < head->getK()) {
			moveChild->setN(head);
			step->second = child;
		}
		else {
			PMODIFY pre = head;
			head = head->getN();
			while (head) {
				if (ck < head->getK())
					break;
				pre = head;
				head = head->getN();
			}
			pre->setN(child);
			moveChild->setN(head);
		}
	}
#ifdef SHOW
	out << "Total: ";
	out << (PMODIFY)(modifyList.find(parent)->second);
	out << "\n";
	out.close();
#endif
	//cout << "Out pushModify!!!\n";
	protectList.unlock();
	//outputModify(parent, (PMODIFY)modifyList.find(parent)->second);//test;
}

template<typename keyType>
void batree<keyType>::outputModify(PNODE parent, PMODIFY child) {
	//cout << "In outputModify\n";
	char name[66];
	sprintf(name, "data\\%d_modify_%x.txt", current, parent);
	ofstream out(name, ios::app);
	out << "Parent: ";
	out << parent;
	out << "\n";
	out << child;
	out.close();
	//cout << "Out outputModify!!!\n";
}

template<typename keyType>
void batree<keyType>::swap(keyType& a, keyType& b) {
	keyType tmp = b;
	b = a;
	a = tmp;
}

template<typename keyType>
void batree<keyType>::showChildB(vector<PNODE>& childBuf) {
	int i = 0, n = childBuf.size();
	for (; i < n; i++) {
		doShow(childBuf[i], 0);
		cout << '\n';
	}
	cout << '\n';
}

template<typename keyType>
void batree<keyType>::showBuffer(vector<keyType>& buffer, vector<PNODE>& child) {
	char name[66];
	if (child.size())
		sprintf(name, "data\\%d_showBuffer_%d_%d.txt", current, child[0], child[0]->getL());
	else
		sprintf(name, "data\\%d_showBuffer_%d_.txt", current, this_thread::get_id());
	ofstream out(name);
	int i = 0, n = buffer.size();
	out <<"buffer "<< n << ": \n";
	for (; i < n; i++)
		out << buffer[i] << " ";
	n = child.size();
	out << "\nchild " << n << ":\n";
	for (i = 0; i < n; i++)
		out << child[i] << '\n';
	out.close();
}

template<typename keyType>
void batree<keyType>::showModifyList(char*	fileName) {
	cout << "In showModifyList: " << fileName << "\n";
	ofstream out(fileName);
	infoIter move = modifyList.begin();
	while (move != modifyList.end()) {
		out << (PNODE)move->first;
		((PNODE)move->first)->show();
		cout << endl;
		out << (PMODIFY)move->second;
		((PMODIFY)move->second)->show();
		cout << endl;
		move++;
	}
	out.close();
	cout << "Out showModifyList!!!\n";
}

template<typename keyType>
void batree<keyType>::find(INDEX p) {	//the supporting funciton
	//cout << "Current thread id is: "
		//<< this_thread::get_id() << " -> " << p << '\n';
	threadsId[p] = this_thread::get_id();
	for (int i = 0; i < EACH_NUM; i++) {
		queries[p][i].setA(search(queries[p][i].getK()));
		//queries[p][i].setA((PNODE)findLeaf(queries[p][i].getK()));
	}
}

template<typename keyType>
void* batree<keyType>::findLeaf(keyType k) {	//get the NODE pointer
	int i;
	PNODE r = root;
	while (r) {
		if (r->getL()) break;
		i = 0;
		if (r->getC(0))//for the leaf NODE
			while (i < r->getN() && k >= r->getK(i)) i++;
		else//for the inner NODE
			while (i < r->getN() && k > r->getK(i)) i++;
		r = r->getC(i);
	}
	return r;
}

template<typename keyType>
bool batree<keyType>::search(keyType k) {	//search k in root
	PNODE r = root;
	while (r) {
		int i = 0;
		if (r->getC(0))//for the leaf NODE
			while (i < r->getN() && k >= r->getK(i)) i++;
		else//for the inner NODE
			while (i < r->getN() && k > r->getK(i)) i++;
		if (i < r->getN() && k == r->getK(i)) {
			//cout << "Here is " << k << '\n';
			return true;
		}
		else if (r->getL()) {
			//cout << "The key does not exist!" << endl;
			return false;
		}
		r = r->getC(i);
	}
	//cout << "The key does not exist!" << endl;
	return false;
}

template<typename keyType>
void batree<keyType>::doShow(ofstream& out, PNODE root, int d) {	//show the nodes in the order of floor
	PNODE tmp = root;
	for (int i = 0; i < d; i++) out << "   ";
	if (d) out << "->";
	out << "(" << tmp->getN() << ": ";
	for (int i = 0; i < tmp->getN(); i++)
		out << tmp->getK(i) << " ";
	out << ")" << endl;
	if (!tmp->getL())
		for (int i = 0; i <= tmp->getN(); i++)
			doShow(out, tmp->getC(i), d + 1);
}

template<typename keyType>
void batree<keyType>::show(int	tag, keyType	key) {//API for showing the btrees
	ofstream out(treeFileName, ios::app);
	out << endl;
	if (tag == 1) {
		out << deep << ": ";
		out << "DEL ";
		out << key << endl;
	}
	else if (tag == 0) {
		out << deep << ": ";
		out << "INS ";
		out << key << endl;
	}
	doShow(out, root, 0);
	out.close();
}

template<typename keyType>
void batree<keyType>::testParent(keyType n) {//API for showing the btrees
	PNODE tmp = (PNODE)findLeaf(n);
	while (tmp) {
		int i, n = tmp->getN();
		for (i = 0; i < n; i++)
			cout << tmp->getK(i) << " ";
		cout << endl;
		tmp = tmp->getP();
	}
}

template<typename keyType>
void batree<keyType>::doClear(PNODE t) {	//show the nodes in the order of floor
	if (t) {
		for (int i = 0; i <= t->getN(); i++)
			doClear(t->getC(i));
		delete t;
	}
}

template<typename keyType>
void batree<keyType>::clear() {//API for free the sources we apply
	//cout << "In clear\n";
	doClear(root);
	//cout << "Out clear!!!\n";
}