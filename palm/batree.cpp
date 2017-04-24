#include "batree.h"

template<typename keyType>
batree<keyType>::batree() {	//initial
	root = new NODE();
	if (!root)
		cout << "The room is not enough!!!!!\n";
	root->setL(true);	//start with leaf
	queries.resize(THREAD_NUM);
	threadsId.resize(THREAD_NUM);
}

template<typename keyType>
batree<keyType>::~batree() {	//free the sources
	//cout << "In clean\n";
	//cout << "IN Clean\n";
	clear();
	//cout << "FUCK!\n";
	//show();//test
	delete root;

	for (int i = 0; i < THREAD_NUM; i++)//clean queries
		queries[i].clear();
	queries.clear();

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
	//cout << "Fuck list\n";

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
	//cout << "Fuck modify\n";

	threads.clear();
	threadsId.clear();
	//cout << "Out clean\n";
}

template<typename keyType>
void batree<keyType>::getTree() {
	node* move = root;
	move->setL(false);
	move->setN(3);
	move->setK(0, 6);
	move->setK(1, 12);
	move->setK(2, 18);
	for (int i = 0, j = 2, k = 0; i <= 3; i++, j += 6) {
		node* tmp = new node(move, 2, false);
		tmp->setK(0, j);
		tmp->setK(1, j + 2);
		for (int p = 0; p < 3; p++) {
			int n = 2;
			if (k == 22) n = 5;
			node* one = new node(tmp, n, true);
			for (int m = 0; m < n; m++)
				one->setK(m, k++);
			tmp->setC(p, one);
		}
		move->setC(i, tmp);
	}
	show();
}

template<typename keyType>
void batree<keyType>::fastRandom() {	//get the query randomly
	//srand((int)time(0));
	//while (queries.size() < EACH_NUM)
	//	queries.push_back(QUERY(rand() % 3, rand() % 9999));

	//get the search query randomly, stored in 'queries'
	for (int i = 0; i < THREAD_NUM; i++)
		for (int j = 0; j < EACH_NUM; j++)
			queries[i].push_back(QUERY(FIN_STEP, rand() % 18));

	//get the Delete&Insert query randomly, store in 'list'
	PNODE p;
	keyType k;
	infoIter item;
	for (int i = 0; i < EACH_NUM*EACH_NUM; i++) {
		k = rand() % 100;	//while keytype is INT
		p = (PNODE)findLeaf(k);
		item = list.find(p);
		if (item != list.end()) {
			PINFO head = (PINFO)item->second, tmp;
			tmp = new INFO((STEP_TYPE)(rand() % 2 + 1), k);
			tmp->setN(head);
			item->second = tmp;
		}
		else
			list[p] = new INFO((STEP_TYPE)(rand() % 2 + 1), k);
	}
}

template<typename keyType>
void batree<keyType>::outputQuery(char* fileName) {	//output the query
	//ofstream out(QUERY_FILE_NAME);
	//unsigned int i, n = queries.size();
	//for (i = 0; i < n; i++)
	//	out << queries[i];
	//out.close();

	ofstream out(fileName);
	for (int i = 0; i < THREAD_NUM; i++) {
		out << i << " - " << threadsId[i] << ": \n";
		for (int j = 0; j < EACH_NUM; j++)
			out << queries[i][j] << '\n';
		out << "\n";
	}
	out.close();
}

template<typename keyType>
void batree<keyType>::outputInfo(char* fileName) {	//output the info
	//ofstream out(QUERY_FILE_NAME);
	//unsigned int i, n = queries.size();
	//for (i = 0; i < n; i++)
	//	out << queries[i];
	//out.close();

	ofstream out(fileName);
	infoIter tmp = list.begin();
	for (; tmp != list.end(); tmp++) {
		out << (PNODE)tmp->first << ": \n";
		out << (PINFO)tmp->second;
	}
	out.close();
}

template<typename keyType>
void batree<keyType>::palm() {	//palm operation for this BPlus tree
	//stage 1: search
	fastRandom();
	outputQuery(QUERY_FILE_NAME);
	outputInfo(INFO_FILE_NAME);
	int i = 0;
	for (; i < THREAD_NUM; i++)
		threads.push_back(thread(&batree<keyType>::find, this, i));
		//sync
	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
	outputQuery(QRESULT_FILE_NAME);
	cout << "finish the search!\n\nStarting modify-leaf-node...\n";

	//stage 2: modify-leaf-NODE
	threads.clear();
	
	infoIter move= list.begin();
	for (i = 0; i < THREAD_NUM && move!=list.end(); i++, move++)
		threads.push_back(thread(&batree<keyType>::modifyNode, this, move, 0));
		//sync
	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));

	for (; i > 0; i--)	//remove the info which is handled
		list.erase(list.begin());

	cout << "finish the modify-leaf-node!\n\nStarting modify-inner-node...\n";
	outputInfo(IRESULT_FILE_NAME);

	//stage 3: modify-inner-NODE
	for (int j = 2, deep = getDeep(); j < deep; j++) {
		cout << "Deep: " << deep << '\n';
		threads.clear();
		move = modifyList.begin();
		for (i = 0; i < THREAD_NUM && move != modifyList.end(); i++, move++)
			threads.push_back(thread(&batree<keyType>::modifyNode, this, move, 1));
		//sync
		for_each(threads.begin(), threads.end(), mem_fn(&thread::join));

		for (; i > 0; i--)	//remove the info which is handled
			modifyList.erase(modifyList.begin());
		cout << "-.-\n";
	}
	cout << "finish the modify-inner-node!\n\nStarting modify-root-node...\n";
	return;

	//stage 4: handle the root
	threads.clear();
	move = modifyList.begin();
	if (move != modifyList.end()) {
		threads.push_back(thread(&batree<keyType>::handleRoot, this));
		threads[0].join();
	}
}

template<typename keyType>
int batree<keyType>::getDeep() {
	int deep = 0;
	for (PNODE tmp = root; tmp; deep++, tmp = tmp->getC(0));
	return deep;
}

template<typename keyType>
void batree<keyType>::handleRoot() {	//the supporting funciton
	cout << this_thread::get_id() << '\n';
}

template<typename keyType>
void batree<keyType>::modifyNode(infoIter inf, INDEX p) {	//the supporting funciton
	cout << "In modify " << p << "\n";

	vector<PNODE> childBuf;
	keyType buffer[TEST_NUM];
	PNODE cur = (PNODE)inf->first;
	int i, n = cur->getN();//just for int
	for (i = 0; i < n; i++)
		buffer[i] = cur->getK(i);
	//showBuffer(buffer, n);//test
	getBuffer(childBuf, inf, buffer, n, p);

	sort(buffer, buffer + n);//sorting 

	//showBuffer(buffer, n);	//test
	//showChildB(childBuf);		//test

	int j = 0, k = 0;
	PMODIFY child = NULL;
	PNODE parent = cur->getP(), buf;
	if (n > MAX_DEGREE) {	//	NEED AHNDLE THE DIFFER 
		//big - split();
		cur->setN(DEGREE);//set the number of key
		child = new MODIFY(UPD_STEP, buffer[j], cur);
		if (p == 1 && inParent(cur, parent) > 0)
			j++;
		child->setO(cur->getK(0));
		PMODIFY moveChild = child;
		while (n > MAX_DEGREE) {
			buf = moveChild->getL();
			for (i = 0; i < MIN_DEGREE; i++, j++) {
				buf->setK(i, buffer[j]);
				buf->setC(i, child[k++]);
			}
			buf->setC(i, child[k++]);
			moveChild->setN(new MODIFY(INS_STEP, buffer[j], new NODE(parent, MIN_DEGREE, cur->getL())));
			if (p == 1)
				j++;
			moveChild = moveChild->getN();
			n -= MIN_DEGREE;
		}
		moveChild->getL()->setN(n);//set the number of key
		buf = moveChild->getL();
		for (i = 0; n > 0; i++, j++, n--) {
			buf->setK(i, buffer[j]);
			buf->setC(i, child[k++]);
		}
		buf->setC(i, child[k]);

		pushModify(parent, child, moveChild);
	}
	else {
		child = new MODIFY(UPD_STEP, buffer[j], cur);
		child->setO(cur->getK(0));
		if (n < MIN_DEGREE) child->setT(DEL_STEP);

		cur->setN(n);//set the number of key
		for (; n > 0; j++, n--) {
			buf->setK(i, buffer[j]);
			buf->setC(i, child[k++]);
		}
		buf->setC(i, child[k]);

		pushModify(parent, child, child);
		//update();
	}
	cout << "Out modify!!! " << p << "\n";
}

template<typename keyType>
int batree<keyType>::inParent(PNODE child, PNODE parent) {	//Done
	int n = parent->getN();
	for (int i = 0; i <= n; i++)
		if (cur == parent->getC(i))
			return i;
	return -1;
}

template<typename keyType>
int batree<keyType>::inBuffer(keyType* buffer, keyType key, int n) {
	int i = 0;
	for (; i < n; i++)
		if (buffer[i] == key)
			return i;
	return -1;
}

template<typename keyType>
void batree<keyType>::getBuffer(vector<PNODE>& child, infoIter inf, keyType* buffer, int& n, INDEX p) {
	if (p == 0) {
		PINFO ope = (PINFO)inf->second, move;
		int i;
		while (ope) {
			keyType key = ope->getK();
			i = inBuffer(buffer, key, n);
			if (ope->getT() == INS_STEP) {
				if (i == n)	//push into the last position
					buffer[n++] = key;
			}
			else if (ope->getT() == DEL_STEP) {
				if (i < n)
					swap(buffer[i], buffer[--n]);
			}
			move = ope->getN();
			delete ope; //free the room
			ope = move;
		}
	}
	else {
		PNODE cur = (PNODE)inf->first;
		PMODIFY ope = (PMODIFY)inf->second, move;
		int i, j = 0;
		while (ope) {
			keyType key = ope->getK();
			move = ope->getN();
			if (ope->getT() == INS_STEP) {
				buffer[n++] = key;
				child.push_back(ope->getL());
				delete ope; //free the room
			}
			else if (ope->getT() == DEL_STEP) {//PROBLEM: for head
				i = inBuffer(buffer, key, n);
				if (ope->getL()->getN() == 0)
					swap(buffer[i], buffer[--n]);
				else {
					child.push_back(ope->getL());	//get the children: j need to tag
					j++;
				}
				delete ope; //free the room
			}
			else if (ope->getT() == UPD_STEP) {	// for head
				if (ope->getL()) {
					i = inParent(ope->getL(), cur);
					if (i >= 0) {		//update in current node
						for (; j < i; j++)
							child.push_back(cur->getC(j));
						child.push_back(ope->getL());	//get the children: j need to tag
						j++;
					}
					else {				//update in inner node
						ope->setL(NULL);
						ope->setN(NULL);
						pushModify(cur->getP(), ope, ope);//store the update info
					}
				}
				i = inBuffer(buffer, ope->getO(), n);
				if (i >= 0) {
					buffer[i] = key;
					delete ope; //free the room
				}
			}
			ope = move;
		}
		soft(child, buffer, n);	//it is import to maintain the structure of BPlus Trees
	}
}

template<typename keyType>
void batree<keyType>::soft(vector<PNODE>& child, keyType* buffer, int& n) {	//BIG TROUBLE
	while (check(child) && n > 1) {
		int i = 1;
		for (; i < n;) {
			if (child[i - 1]getN() + child[i]->getN() <= MAX_DEGREE) {	//merge, shiftLTR, shiftRTL
				merge(buffer[i - 1], child[i - 1], child[i]);
				child.erase(child.begin() + i);
				n--;
			}
			else i++;
		}
	}
	//showChildB(child);//test for watching the content of child
}

template<typename keyType>
int batree<keyType>::check(vector<PNODE>& child) {
	int i = 0, n = child.size();
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
}

template<typename keyType>
void batree<keyType>::merge(keyType key, PNODE y, PNODE z) {
	//y - left node, z - right node
	int i = y->getN(), j = 0, n = z->getN();
	if (!y->getL())	//inner node
		y->setK(i++, key);
	for (; j < n; i++, j++)
		y->setK(i, z->getK(j));
	if (!y->getL()) {	//inner node
		for (i = y->getN() + 1, j = 0; j <= n; j++, i++)
			y->setC(i, z->getC(j));
	}
	if (!y->getL())	//inner node
		y->setN(y->getN() + n + 1);
	else
		y->setN(y->getN() + n);
	delete z;
	//show();//test
}

template<typename keyType>
void batree<keyType>::shiftLTR(keyType& key, int n, PNODE y, PNODE z) {
	//z borrows n nodes from y
	int i = z->getN();
	z->setN(i + n);
	z->setC(i + n, z->getC(i));
	for (--i; i >= 0; i--) {
		z->setK(i + n, z->getK(i));
		z->setC(i + n, z->getC(i));
	}

	int k = y->getN(), j = n - 1;
	y->setN(k - n);
	i = k - 1;
	if (!y->getL()) { //inner node
		z->setK(j, key);
		z->setC(j, y->getC(k));
		j--, k--;
	}
	for (; j >= 0; i--, j--, k--) {
		z->setK(j, y->getK(i));
		z->setC(j, y->getC(k));
	}
	if (!y->getL()) //inner node
		key = y->getK(i);
	else
		key = z->getK(0);
}

template<typename keyType>
void batree<keyType>::shiftRTL(keyType& key, int n, PNODE y, PNODE z) {
	//y borrows n nodes from z
	int i = y->getN(), move = 0, j = 0, k = 0;
	y->setN(i + n);
	if (!y->getL()) { //inner
		y->setK(i, key);
		y->setC(i + 1, z->getC(k));
		i++, k++;
		move++;
	}
	for (; move < n; move++, i++, j++, k++) {
		y->setK(i, z->getK(j));
		y->setC(i + 1, z->getC(k));
	}
	key = z->getK(j);
	if (!y->getL())
		j++;
	i = z->getN();
	z->setN(i - n);
	for (; j < i; j++, k++) {
		z->setK(j - n, z->getK(j));
		z->setC(j - n, z->getC(k));
	}
	z->setC(j - n, z->getC(k));
}

template<typename keyType>
void batree<keyType>::pushModify(PNODE parent, PMODIFY child, PMODIFY moveChild){
	lock_guard<mutex> guard(protectList);	//protect the modifyList
	//outputModify(parent, child);//test;
	infoIter step = modifyList.find(parent);
	if (step == modifyList.end())
		modifyList[parent] = child;
	else {
		PMODIFY head = (PMODIFY)step->second;
		keyType ck = child->getK();
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
	//outputModify(parent, (PMODIFY)modifyList.find(parent)->second);//test;
}

template<typename keyType>
void batree<keyType>::outputModify(PNODE parent, PMODIFY child) {
	char name[30];
	sprintf(name, "%d.txt", parent);
	ofstream out(name, ios::app);
	out << parent <<" is being modified!\n";
	out << child;
	out.close();
	return;
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
void batree<keyType>::showBuffer(keyType* buffer, int n) {
	int i = 0;
	for (; i < n; i++)
		cout << buffer[i] << " ";
	cout << '\n';
}

template<typename keyType>
void batree<keyType>::find(INDEX p) {	//the supporting funciton
	cout << "Current thread id is: "
		<< this_thread::get_id() << " -> " << p << '\n';
	threadsId[p] = this_thread::get_id();
	for (int i = 0; i < EACH_NUM; i++)
		queries[p][i].setA((PNODE)findLeaf(queries[p][i].getK()));
}

template<typename keyType>
void* batree<keyType>::findLeaf(keyType k) {	//get the NODE pointer
	PNODE r = root;
	while (r) {
		int i = 0;
		if (r->getC(0))//for the leaf NODE
			while (i < r->getN() && k >= r->getK(i)) i++;
		else//for the inner NODE
			while (i < r->getN() && k > r->getK(i)) i++;
		if (r->getL())
			break;
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

//template<typename keyType>
//void batree<keyType>::split(PNODE x, int i) {	//split the child whose index is i of NODE x
//	//x - current NODE, i - the index of NODE which will be splited
//	int len = MIN_DEGREE, basis = 0;//for the iner NODE
//	PNODE z = new NODE(), y = x->getC(i);
//	z->setP(x);//set the parent of z
//	z->setL(y->getL());
//	if (z->getL()) {//for the leaf NODE
//		len++;
//		basis = -1;
//	}
//	z->setN(len);
//	for (int j = 0; j < len; j++)
//		z->setK(j, y->getK(j + DEGREE + basis));
//	if (y->leaf == false)
//		for (int j = 0; j <= len; j++) {
//			z->setC(j, y->getC(j + DEGREE + basis));
//			z->getC(j)->setP(z);//set the parent
//		}
//	y->setN(MIN_DEGREE);
//	//y->show();//test
//	//z->show();//test
//	for (int j = (int)x->getN(); j > i; j--)
//		x->setC(j + 1, x->getC(j));
//	x->setC(i + 1, z);
//	for (int j = (int)x->getN() - 1; j >= i; j--)
//		x->setK(j + 1, x->getK(j));
//	x->setK(i, y->getK(MIN_DEGREE));
//	x->setN(x->getN() + 1);
//	//x->show(0);//test
//	//cout << endl;//test
//}
//
//template<typename keyType>
//void batree<keyType>::insertNon(PNODE x, keyType k) {	//insert the k into the subtree whose root is NODE x
//	int i = x->getN() - 1;
//	if (x->leaf) {
//		while (i >= 0 && k < x->getK(i)) {
//			x->setK(i + 1, x->getK(i));
//			i--;
//		}
//		x->setK(i + 1, k);
//		x->setN(x->getN() + 1);
//	}
//	else {
//		while (i >= 0 && k < x->getK(i)) i--;
//		i++;
//		if (x->getC(i)->getN() == MAX_DEGREE) {
//			split(x, i);
//			if (k > x->getK(i)) i++;
//		}
//		insertNon(x->getC(i), k);
//	}
//}
//
//template<typename keyType>
//void batree<keyType>::insert(keyType k) {	//insert the k into root
//	//Before inserting, we split the full NODE
//	if (search(k)) {
//		cout << k << " Is Already Here!" << endl;
//		return;
//	}
//	PNODE r = root;
//	if (MAX_DEGREE == r->getN()) {
//		PNODE s = new NODE();
//		root = s;
//		s->setN(0);
//		s->setL(false);
//		s->setC(0, r);
//		r->setP(s);//set the parent of r
//		split(s, 0);
//		insertNon(s, k);
//	}
//	else insertNon(r, k);
//}
//
//template<typename keyType>
//void batree<keyType>::del(keyType k) {	//delete the k from root
//	if (search(k)) {
//		PNODE r = root;
//		if (r->getN() == 1 && !r->getL()) {
//			PNODE y = root->getC(0);
//			PNODE z = root->getC(1);
//			int n = y->getN() + z->getN();
//			if (n >= MAX_DEGREE - 1 && n < MAX_DEGREE + 1) {
//				//merge(r, 0, y, z);
//				root = y;
//				delete r;
//				delNon(y, k);
//			}
//			else delNon(r, k);
//		}
//		else delNon(r, k);
//	}
//}
//
//template<typename keyType>
//void batree<keyType>::delNon(PNODE x, keyType k) {
//	int i = 0;
//	if (x->getC(0))//for the leaf NODE
//		while (i < x->getN() && k >= x->getK(i)) i++;
//	else//for the inner NODE
//		while (i < x->getN() && k > x->getK(i)) i++;
//	if (x->getL()) {//Reach the leaf NODE
//		cout << "Cur: " << x->getK(i) << endl;
//		if (k == x->getK(i)) {
//			for (int j = i + 1; j < x->getN(); j++)
//				x->setK(j - 1, x->getK(j));
//			x->setN(x->getN() - 1);
//			if (i == 0) delSet(k, x->getK(0));//reset the head - IMPERATIVE
//		}
//		else cout << "The key does not exist!" << endl;
//		return;
//	}
//	// the iner NODE
//	PNODE z = NULL, p = NULL, y = x->getC(i);
//	if (i < x->getN()) z = x->getC(i + 1);
//	if (i > 0) p = x->getC(i - 1);
//	if (y->getN() == MIN_DEGREE) {
//		if (i > 0 && p->getN() >= DEGREE)//Get: try the left side
//			shiftLTR(x, i - 1, p, y);
//		else if (i < x->getN() && z->getN() >= DEGREE)//Get: try the right side
//			shiftRTL(x, i, y, z);
//		else if (i > 0) {//Merge: try the left side
//			//merge(x, i - 1, p, y);
//			y = p;
//		}
//		//else merge(x, i, y, z);//Merge: try the right side
//	}
//	delNon(y, k);
//}
//
//template<typename keyType>
//void batree<keyType>::delSet(keyType k, keyType v) {	//reset value accoding to the head in inner NODE
//	PNODE r = root;
//	while (r) {
//		int i = 0;
//		while (i < r->getN() && k > r->getK(i)) i++;
//		if (i < r->getN() && k == r->getK(i)) {
//			r->setK(i, v);
//			return;
//		}
//		r = r->getC(i);
//	}
//}

template<typename keyType>
void batree<keyType>::doShow(PNODE root, int d) {	//show the nodes in the order of floor
	PNODE tmp = root;
	for (int i = 0; i < d; i++) cout << "   ";
	if (d) cout << "->";
	cout << "(" << tmp->getN() << ": ";
	for (int i = 0; i < tmp->getN(); i++)
		cout << tmp->getK(i) << " ";
	cout << ")" << endl;
	if (!tmp->getL())
		for (int i = 0; i <= tmp->getN(); i++)
			doShow(tmp->getC(i), d + 1);
}

template<typename keyType>
void batree<keyType>::show() {//API for showing the btrees
	doShow(root, 0);
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
void batree<keyType>::doClear(PNODE root) {	//show the nodes in the order of floor
	if (!root->getL())
		for (int i = 0; i <= root->getN(); i++) {
			doClear(root->getC(i));
			root->setL(true);
			delete root->getC(i);
		}
}

template<typename keyType>
void batree<keyType>::clear() {//API for free the sources we apply
	doClear(root);
}