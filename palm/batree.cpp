#include "batree.h"

template<typename keyType>
batree<keyType>::batree() {	//initial
	cout << "in initial\n";
	root = new NODE();
	if (!root)
		cout << "The room is not enough!!!!!\n";
	cout << "After node\n";
	root->setL(true);	//start with leaf
	queries.resize(THREAD_NUM);
	moveList.resize(THREAD_NUM);
	threadsId.resize(THREAD_NUM);
	cout << "out initial\n";
}

template<typename keyType>
batree<keyType>::~batree() {	//free the sources
	//cout << "In clean\n";
	clear();
	//show();//test
	delete root;

	for (int i = 0; i < THREAD_NUM; i++)//clean queries
		queries[i].clear();
	queries.clear();

	moveList.clear();

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

	threads.clear();
	threadsId.clear();
	//cout << "Out clean\n";
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
	for (int i = 0; i < THREAD_NUM; i++)
		threads.push_back(thread(&batree<keyType>::find, this, i));
	//sync
	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
	outputQuery(QRESULT_FILE_NAME);
	cout << "finish the search!\n\nStarting modify-leaf-node...\n";

	//stage 2: modify-leaf-NODE
	threads.clear();
	
	infoIter move= list.begin();
	for (int i = 0; i < THREAD_NUM && move!=list.end(); i++, move++)
		threads.push_back(thread(&batree<keyType>::modifyNode, this, move, i));
	//sync
	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));

	move = list.begin();
	for (int i = 0; i < THREAD_NUM && move != list.end(); i++) {
		list.erase(move);
		move = list.begin();
	}

	cout << "finish the modify-leaf-node!\n\nStarting modify-inner-node...\n";

	outputInfo(IRESULT_FILE_NAME);
	return;

	//stage 3: modify-inner-NODE
	threads.clear();
	for (int i = 0; i < THREAD_NUM; i++)
		threads.push_back(thread(&batree<keyType>::search, this, i));
	//sync
	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));

	//stage 4: handle the root
	threads.push_back(thread(&batree<keyType>::search, this, 0));
}

template<typename keyType>
void batree<keyType>::modifyNode(infoIter inf, INDEX p) {	//the supporting funciton
	//cout << "In position " << p << "\n";
	cout << "Current thread id is: " << this_thread::get_id() << "\n";

	PNODE cur = (PNODE)inf->first;
	PINFO ope = (PINFO)inf->second;

	keyType buffer[TEST_NUM];
	int i, n = cur->getN();//just for int
	for (i = 0; i < n; i++)
		buffer[i] = cur->getK(i);
	//showBuffer(buffer, n);//test
	PINFO move;
	while (ope) {
		keyType key = ope->getK();
		i = inBuffer(buffer, key, n);
		if (ope->getT() == INS_STEP) {
			//cout << "insert " << key << ": ";//test
			if (i == n)	//push into the last position
				buffer[n++] = key;
		}
		else {
			//cout << "delete " << key << ": ";//test
			if (i < n)
				swap(buffer[i], buffer[--n]);
		}
		move = ope->getN();
		delete ope; //free the room
		ope = move;
		//showBuffer(buffer, n);//test
	}
	//cout << "Out position " << p << "\n";
}

template<typename keyType>
int batree<keyType>::inBuffer(keyType* buffer, keyType key, int n) {
	int i = 0;
	for (; i < n; i++)
		if (buffer[i] == key)
			break;
	return i;
}

template<typename keyType>
void batree<keyType>::swap(keyType& a, keyType& b) {
	keyType tmp = b;
	b = a;
	a = tmp;
}

template<typename keyType>
void batree<keyType>::showBuffer(keyType* buffer, int n) {
	int i = 0;
	for (; i < n; i++)
		cout << buffer[i] << " ";
	cout << '\n';
}

template<typename keyType>
void batree<keyType>::sync() {	//the supporting funciton
	cout << k << " -> " << this_thread::get_id() << '\n';
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

template<typename keyType>
void batree<keyType>::split(PNODE x, int i) {	//split the child whose index is i of NODE x
	//x - current NODE, i - the index of NODE which will be splited
	int len = DEGREEA - 1, basis = 0;//for the iner NODE
	PNODE z = new NODE(), y = x->getC(i);
	z->setP(x);//set the parent of z
	z->setL(y->getL());
	if (z->getL()) {//for the leaf NODE
		len++;
		basis = -1;
	}
	z->setN(len);
	for (int j = 0; j < len; j++)
		z->setK(j, y->getK(j + DEGREEA + basis));
	if (y->leaf == false)
		for (int j = 0; j <= len; j++) {
			z->setC(j, y->getC(j + DEGREEA + basis));
			z->getC(j)->setP(z);//set the parent
		}
	y->setN(DEGREEA - 1);
	//y->show();//test
	//z->show();//test
	for (int j = (int)x->getN(); j > i; j--)
		x->setC(j + 1, x->getC(j));
	x->setC(i + 1, z);
	for (int j = (int)x->getN() - 1; j >= i; j--)
		x->setK(j + 1, x->getK(j));
	x->setK(i, y->getK(DEGREEA - 1));
	x->setN(x->getN() + 1);
	//x->show(0);//test
	//cout << endl;//test
}

template<typename keyType>
void batree<keyType>::insertNon(PNODE x, keyType k) {	//insert the k into the subtree whose root is NODE x
	int i = x->getN() - 1;
	if (x->leaf) {
		while (i >= 0 && k < x->getK(i)) {
			x->setK(i + 1, x->getK(i));
			i--;
		}
		x->setK(i + 1, k);
		x->setN(x->getN() + 1);
	}
	else {
		while (i >= 0 && k < x->getK(i)) i--;
		i++;
		if (x->getC(i)->getN() == 2 * DEGREEA - 1) {
			split(x, i);
			if (k > x->getK(i)) i++;
		}
		insertNon(x->getC(i), k);
	}
}

template<typename keyType>
void batree<keyType>::insert(keyType k) {	//insert the k into root
	//Before inserting, we split the full NODE
	if (search(k)) {
		cout << k << " Is Already Here!" << endl;
		return;
	}
	PNODE r = root;
	if (2 * DEGREEA - 1 == r->getN()) {
		PNODE s = new NODE();
		root = s;
		s->setN(0);
		s->setL(false);
		s->setC(0, r);
		r->setP(s);//set the parent of r
		split(s, 0);
		insertNon(s, k);
	}
	else insertNon(r, k);
}

template<typename keyType>
void batree<keyType>::merge(PNODE x, INDEX i, PNODE y, PNODE z) {
	//i: the index of key in x, y: left child of x, z: right child of x
	int j = DEGREEA, basis = 0, len = 2 * DEGREEA - 1;
	if (y->getL()) {
		j--;
		basis++;
		len = y->getN() + z->getN();
	}
	y->setN(len);
	for (; j < len; j++)
		y->setK(j, z->getK(j - DEGREEA + basis));
	if (!y->getL()) {
		y->setK(DEGREEA - 1, x->getK(i));
		for (j = DEGREEA; j < len + 1; j++)
			y->setC(j, z->getC(j - DEGREEA));
	}
	for (j = i + 1; j < x->getN(); j++) {
		x->setK(j - 1, x->getK(j));
		x->setC(j, x->getC(j + 1));
	}
	x->setN(x->getN() - 1);
	delete z;
	//show();//test
}

template<typename keyType>
void batree<keyType>::del(keyType k) {	//delete the k from root
	if (search(k)) {
		PNODE r = root;
		if (r->getN() == 1 && !r->getL()) {
			PNODE y = root->getC(0);
			PNODE z = root->getC(1);
			int n = y->getN() + z->getN();
			if (n >= 2 * DEGREEA - 2 && n < 2 * DEGREEA) {
				merge(r, 0, y, z);
				root = y;
				delete r;
				delNon(y, k);
			}
			else delNon(r, k);
		}
		else delNon(r, k);
	}
}

template<typename keyType>
void batree<keyType>::delNon(PNODE x, keyType k) {
	int i = 0;
	if (x->getC(0))//for the leaf NODE
		while (i < x->getN() && k >= x->getK(i)) i++;
	else//for the inner NODE
		while (i < x->getN() && k > x->getK(i)) i++;
	if (x->getL()) {//Reach the leaf NODE
		cout << "Cur: " << x->getK(i) << endl;
		if (k == x->getK(i)) {
			for (int j = i + 1; j < x->getN(); j++)
				x->setK(j - 1, x->getK(j));
			x->setN(x->getN() - 1);
			if (i == 0) delSet(k, x->getK(0));//reset the head - IMPERATIVE
		}
		else cout << "The key does not exist!" << endl;
		return;
	}
	// the iner NODE
	PNODE z = NULL, p = NULL, y = x->getC(i);
	if (i < x->getN()) z = x->getC(i + 1);
	if (i > 0) p = x->getC(i - 1);
	if (y->getN() == DEGREEA - 1) {
		if (i > 0 && p->getN() >= DEGREEA)//Get: try the left side
			shiftLTR(x, i - 1, p, y);
		else if (i < x->getN() && z->getN() >= DEGREEA)//Get: try the right side
			shiftRTL(x, i, y, z);
		else if (i > 0) {//Merge: try the left side
			merge(x, i - 1, p, y);
			y = p;
		}
		else merge(x, i, y, z);//Merge: try the right side
	}
	delNon(y, k);
}

template<typename keyType>
void batree<keyType>::delSet(keyType k, keyType v) {	//reset value accoding to the head in inner NODE
	PNODE r = root;
	while (r) {
		int i = 0;
		while (i < r->getN() && k > r->getK(i)) i++;
		if (i < r->getN() && k == r->getK(i)) {
			r->setK(i, v);
			return;
		}
		r = r->getC(i);
	}
}
template<typename keyType>
void batree<keyType>::shiftLTR(PNODE x, INDEX i, PNODE y, PNODE z) {//x's right child y borrows a key and a child from x's left child of z
	//i: the index of key in x, y: left child of x, z: right child of x
	int j = z->getN();
	for (; j > 0; j--)
		z->setK(j, z->getK(j - 1));
	if (y->getL()) {
		z->setK(0, y->getK(y->getN() - 1));
		x->setK(i, z->getK(0));
	}
	else {
		z->setK(0, x->getK(i));
		x->setK(i, y->getK(y->getN() - 1));
	}
	if (!z->getL()) {
		for (j = z->getN(); j >= 0; j--)
			z->setC(j + 1, z->getC(j));
		z->setC(0, y->getC(y->getN()));
	}
	z->setN(z->getN() + 1);
	y->setN(y->getN() - 1);
}

template<typename keyType>
void batree<keyType>::shiftRTL(PNODE x, INDEX i, PNODE y, PNODE z) {//...
	//i: the index of key in x, y: left child of x, z: right child of x
	int n = y->getN();
	if (y->getL()) {
		y->setK(n, z->getK(0));
		x->setK(i, z->getK(1));
	}
	else {
		y->setK(n, x->getK(i));
		x->setK(i, z->getK(0));
	}
	for (int k = 1; k < z->getN(); k++)
		z->setK(k - 1, z->getK(k));
	y->setC(n + 1, z->getC(0));
	if (!z->getL())
		for (int k = 1; k <= z->getN(); k++)
			z->setC(k - 1, z->getC(k));
	y->setN(n + 1);
	z->setN(z->getN() - 1);
}

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
void batree<keyType>::test(keyType n) {//API for showing the btrees
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