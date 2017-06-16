#include "batree.h"

template<typename keyType>
batree<keyType>::batree(int v) {	//initial
	root = new NODE(NULL, 0, true);
	queries.resize(THREAD_NUM);

	variable = 20 * v;
	sprintf(treeFileName, "bPlus%d.txt", variable*THREAD_NUM);
	ofstream out(treeFileName);
	out.close();
	ofstream out2(QUERY_FILE_NAME);
	out2.close();

	costT = 0;
}

template<typename keyType>
batree<keyType>::~batree() {	//free the sources
	clear();
}

template<typename keyType>
void batree<keyType>::getTree() {
	//node* move = root;
	//move->setL(false);
	//move->setN(3);
	//move->setK(0, 6);
	//move->setK(1, 12);
	//move->setK(2, 18);
	//for (int i = 0, j = 2, k = 0; i <= 3; i++, j += 6) {
	//	node* tmp = new node(move, 2, false);
	//	tmp->setK(0, j);
	//	tmp->setK(1, j + 2);
	//	for (int p = 0; p < 3; p++) {
	//		int n = 2;
	//		if (k == 22) n = 5;
	//		node* one = new node(tmp, n, true);
	//		for (int m = 0; m < n; m++)
	//			one->setK(m, k++);
	//		tmp->setC(p, one);
	//	}
	//	move->setC(i, tmp);
	//}
	for (int i = 0; i < THREAD_NUM; i++)
		for (int j = 0; j < EACH_NUM; j++) {
			queries[i].push_back(QUERY(INS_STEP, rand() % 99));
			search(i, j);
			insert(i, j);
		}
	show();
}

template<typename keyType>
void batree<keyType>::fastRandom() {	//get the query randomly
	cout << "In fastRandom\n";
	int i, j;
#ifdef TESTBTT
	for (i = 0; i < THREAD_NUM; i++)
		for (j = 0; j < EACH_NUM; j++) {
			queries[i].push_back(QUERY(INS_STEP, rand() % 1000));
			search(i, j);
			insert(i, j);
		}
	queries[0][0].key = 25;
	search(0, 0);
	insert(0, 0);

	int n;
	cout << "Input the number of test cases: ";
	queries[0][0].type = DEL_STEP;
	//queries[0][0].key = 478;
	//search(0, 0);
	//del(0, 0);
	cin >> n;
	for (; n > 0; n--) {
		cin >> i;
		queries[0][0].key = i;
		search(0, 0);
		del(0, 0);
		testParent(999);
	}
#else
#ifdef SYNC
	for (i = 0; i < THREAD_NUM; i++)
		for (j = 0; j < EACH_NUM; j++)
			queries[i].push_back(QUERY(FIN_STEP, rand() % 99));
#else
	for (i = 0; i < THREAD_NUM; i++)
		for (j = 0; j < EACH_NUM; j++) {
			queries[i].push_back(QUERY((STEP_TYPE)(rand() % 3), rand() % 99));
		}
#endif

	outputQuery(QUERY_FILE_NAME);

#ifdef SYNC
	cout << "Start searching...\n";
	clk.start();
	for (j = 0; j < EACH_NUM; j++) {
		for (i = 0; i < THREAD_NUM; i++)
			threads.push_back(thread(&batree<keyType>::search, this, i, j));
		//sync
		for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
		threads.clear();
	}
	costT += clk.stop();
	cout << "Finishing searching!!!\n";

	keyType		k;
	STEP_TYPE	s;
	for (i = 0; i < EACH_NUM*EACH_NUM; i++) {
		k = rand() % 99;	//while keytype is INT
		s = (STEP_TYPE)(rand() % 2 + 1);
		QUERY t(s, k);
		searchSync(t);
		container.push(t);
	}

	cout << "Start modifying...\n";
	clk.start();
	while (!container.empty()) {
		for (i = 0; i < THREAD_NUM && !container.empty(); i++) {
			QUERY tmp = container.top();
			if (tmp.type == INS_STEP)
				threads.push_back(thread(&batree<keyType>::insertSync, this, tmp));
			else
				threads.push_back(thread(&batree<keyType>::delSync, this, tmp));
			container.pop();
		}
		for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
		threads.clear();
	}
	costT += clk.stop();
	cout<<"Finish modifying!!!\n";
#else
	cout << "Start searching...\n";
	for (j = 0; j < EACH_NUM; j++) {
		clk.start();
		for (i = 0; i < THREAD_NUM; i++)
			threads.push_back(thread(&batree<keyType>::search, this, i, j));
		//sync
		for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
		costT += clk.stop();
		threads.clear();
	}
	outputQuery(QRESULT_FILE_NAME);
	cout << "Finishing searching!!!\n";

	for (i = 0; i < THREAD_NUM; i++) {
		queries[i].clear();
		for (j = 0; j < variable; j++)
			queries[i].push_back(QUERY((STEP_TYPE)(rand() % 2 + 1), rand() % 99));
	}

	for (j = 0; j < variable; j++) {
		//cout << "Mid fastRandom:" << j << "\n";

		clk.start();
		for (i = 0; i < THREAD_NUM; i++)
			threads.push_back(thread(&batree<keyType>::search, this, i, j));
		for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
		costT += clk.stop();
		threads.clear();

		//cout << "Finish the search " << j << '\n';
		clk.start();
		for (i = 0;i<THREAD_NUM; i++) {
			if (queries[i][j].type == INS_STEP)
				threads.push_back(thread(&batree<keyType>::insert, this, i, j));
			else
				threads.push_back(thread(&batree<keyType>::del, this, i, j));
		}
		//mem_fn(&thread::join);
		for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
		costT += clk.stop();
		threads.clear();
		//cout << "Out Mid " << j << "!!!\n";
		show(j);
	}
#endif

	ofstream out("data.txt", ios::app);
	out << variable*THREAD_NUM << "\tYour program executed time is " << costT << "ms.\n";
	out.close();
#endif
	cout << "Out fastRandom!!!\n";
}

template<typename keyType>
void batree<keyType>::outputQuery(char* fileName) {	//output the query
	ofstream out(fileName);
	for (int i = 0; i < THREAD_NUM; i++) {
		out << i << ": \n";
		for (int j = 0; j < EACH_NUM; j++)
			out << queries[i][j] << '\n';
		out << "\n";
	}
	if (strcmp(fileName, QUERY_FILE_NAME) == 0)
		out << "\nStarting the process:\n";
	out.close();
}

template<typename keyType>
void* batree<keyType>::findLeaf(PNODE r, keyType k) {	//get the NODE pointer
	while (r) {
		if (r->getL())
			break;
		int i = 0;
		if (r->getC(0))//for the leaf NODE
			while (i < r->getN() && k >= r->getK(i)) i++;
		else//for the inner NODE
			while (i < r->getN() && k > r->getK(i)) i++;
		r = r->getC(i);
	}
	return r;
}

template<typename keyType>
void batree<keyType>::search(int	x, int	y) {	//search k in root
	//cout << "In search\n";
	keyType k = queries[x][y].getK();
	queries[x][y].setA(false);
	PNODE r = root;
	while (r) {
		int i = 0;
		if (r->getC(0))//for the leaf node
			while (i < r->getN() && k >= r->getK(i)) i++;
		else//for the inner node
			while (i < r->getN() && k > r->getK(i)) i++;
		if (i < r->getN() && k == r->getK(i)) {
			queries[x][y].setA(true);
			if (queries[x][y].type == FIN_STEP || queries[x][y].type == INS_STEP)
				return;
		}
		if (r->getL())
			break;
		r = r->getC(i);
	}
	if (queries[x][y].type != FIN_STEP) {
		if (r->parent)
			queries[x][y].setC(r->parent);
		else
			queries[x][y].setC(r);
		//cout << "Got: ";	//test
		//queries[x][y].getC()->show();	//test
		//cout<<endl;	//test
	}
#ifdef DEBUG_
	queries[x][y].getC()->show();	//debug
	cout << endl;	//debug
#endif
	//cout << "Out search!!!\n";
}

#ifdef SYNC
template<typename keyType>
void batree<keyType>::searchSync(QUERY& t) {	//search k in root
	keyType k = t.getK();
	t.setA(false);
	PNODE r = root;
	while (r) {
		int i = 0;
		if (r->getC(0))//for the leaf node
			while (i < r->getN() && k >= r->getK(i)) i++;
		else//for the inner node
			while (i < r->getN() && k > r->getK(i)) i++;
		if (i < r->getN() && k == r->getK(i)) {
			t.setA(true);
			if (t.type == FIN_STEP || t.type == INS_STEP)
				return;
		}
		if (r->getL())
			break;
		r = r->getC(i);
	}
	if (t.type != FIN_STEP) {
		if (r->parent)
			t.setC(r->parent);
		else
			t.setC(r);
	}
}
#endif

template<typename keyType>
void batree<keyType>::split(stack<PNODE>& path, PNODE x, int i) {	//split the child whose index is i of node x
	//x - current node, i - the index of node which will be splited
	//x->show();
	//cout << ": " << i << endl;
	int len = MIN_DEGREE, basis = 0;//for the iner node
	PNODE z = new NODE(x), y = x->getC(i);
	z->setL(y->getL());
	if (z->getL()) {//for the leaf node
		len++;
		basis = -1;
	}
	z->setN(len);
	for (int j = 0; j < len; j++)
		z->setK(j, y->getK(j + DEGREE + basis));
	if (y->leaf == false)
		for (int j = 0; j <= len; j++) {
			z->setC(j, y->getC(j + DEGREE + basis));
			z->getC(j)->setP(z);
		}
	y->setN(MIN_DEGREE);
	for (int j = x->getN(); j > i; j--)
		x->setC(j + 1, x->getC(j));
	x->setC(i + 1, z);
	for (int j = x->getN() - 1; j >= i; j--)
		x->setK(j + 1, x->getK(j));
	x->setK(i, y->getK(MIN_DEGREE));
	x->setN(x->getN() + 1);
	if (!path.empty()) {
		x->curLock.unlock();	//lock
		if (y != path.top()->parent) {
			y->curLock.unlock();	//lock
			z->curLock.lock();	//lock
		}
	}
	//cout << endl;//test
}

template<typename keyType>
void* batree<keyType>::splitPath(PNODE r, keyType k) {
	//cout << "In splitPath\n";
	stack<PNODE>				path;		//store the path from BOTTOM up to TOP
	PNODE tmp = (PNODE)findLeaf(r, k);
	if (tmp != r) {
		tmp->curLock.lock();	//lock
		if (tmp->getN() == MAX_DEGREE)
			path.push(tmp);
		else {
			r->curLock.unlock();	//unlock
			//cout << "Out splitPath!!!\n";
			return tmp;
		}
	}
	while (r && r->getN() == MAX_DEGREE) {
		path.push(r);
		r = r->parent;
		if (r)
			r->curLock.lock();	//lock the current node in the path from bottom up to top
	}
	if (path.empty()) {
		//cout << "Out splitPath!!!\n";
		return r;
	}
	r = path.top();
	if (!r->parent) {
		r = new NODE();
		r->curLock.lock();		//lock
		r->setC(0, root);
		root->setP(r);
		root = r;
	}
	while (!path.empty()) {
		r = path.top();
		path.pop();

		//cout << "Split: ";
		//r->show();
		//cout << endl;
		
		split(path, r->parent, r->getI());
		//cout << "Finish Split\n";
	}
	PNODE ans = (PNODE)findLeaf(r->parent, k);
	//cout << "-.-\n";
	r->parent->curLock.unlock();
	//cout << "P.unlock\n";
	r->curLock.unlock();	//unlock
	//cout << "r.unlock\n";
	ans->curLock.lock();	//lock
	//cout << "Out splitPath!!!\n";
	return ans;
}

template<typename keyType>
void batree<keyType>::insert(int	x, int	y) {	//insert the k into root
	//Before inserting, we split the full node
	keyType k = queries[x][y].getK();
	if (queries[x][y].ans)
		return;
	PNODE r = queries[x][y].lock();

	//cout << "\t ins " << k << "\n";	//test
	//cout << "Parent: ";	//test
	//r->show();	//test
	//cout << "\n";	//test

	r = (PNODE)splitPath(r, k);

	int i = r->getN() - 1;
	while (i >= 0 && k < r->getK(i)) {
		r->setK(i + 1, r->getK(i));
		i--;
	}
	r->setK(i + 1, k);
	r->setN(r->getN() + 1);

	//cout << "\tWe insed: ";	//test
	//r->show();	//test
	//cout << "\n";	//test
	
	r->curLock.unlock();	//lock
#ifdef TESTBTT
	show(x, y);
#endif
	//cout << "Out insert!!!\n";
}

#ifdef SYNC
template<typename keyType>
void batree<keyType>::insertSync(QUERY&	t) {	//insert the k into root
	//Before inserting, we split the full node
	keyType k = t.getK();
	if (t.ans)
		return;
	PNODE r = t.lock();
	r = (PNODE)splitPath(r, k);
	int i = r->getN() - 1;
	while (i >= 0 && k < r->getK(i)) {
		r->setK(i + 1, r->getK(i));
		i--;
	}
	r->setK(i + 1, k);
	r->setN(r->getN() + 1);

	r->curLock.unlock();	//lock
}
#endif

template<typename keyType>
bool batree<keyType>::company(stack<PNODE>& path, PNODE& r, PNODE& ans) {
	//cout << "In company\n";
	//cout << "\tr: ";
	//r->show();
	//cout << endl;
	if (r->parent) {	//Non root
		if (!r->getL())
			r->parent->curLock.lock();	//lock
		if (r->getN() > MIN_DEGREE) {
			if (r->parent != ans)
				r->parent->curLock.unlock();	//unlock
			//cout << "Out company!!!\n";
			return true;
		}
		PNODE x = r->parent;
		int i = r->getI(), len = x->getN();
		if (i > 0 && x->getC(i - 1)->getN() > MIN_DEGREE) {
			shiftLTR(x, i - 1, x->getC(i - 1), r);
			if (x != ans)
				x->curLock.unlock();	//unlock
			//cout << "Out company!!!\n";
			return true;
		}
		else if (i < len && x->getC(i + 1)->getN() > MIN_DEGREE) {
			shiftRTL(x, i, r, x->getC(i + 1));
			if (x != ans)
				x->curLock.unlock();	//unlock
			//cout << "Out company!!!\n";
			return true;
		}
		//cout << "Out company!!!\n";
		return false;
	}
	//for the root
	//cout << "Root\n";
	if (r->getN() == 1 && !r->getL()) {
		merge(path, r, 0, r->getC(0), r->getC(1));
		root = r->getC(0);
		root->setP(NULL);
		if (!path.empty()) {
			path.pop();
			path.push(root);
		}
		if (r == ans)
			ans = root;
		r->curLock.unlock();	//unlock
		delete r;
		r = NULL;
	}
	//cout << "Out company!!!\n";
	return true;
}

template<typename keyType>
void batree<keyType>::getNode(PNODE r, PNODE& ans, bool& tag, keyType k) {
	if (k >= r->getK(0) && k <= r->getK(r->getN() - 1)) {
		tag = true;
		ans = r;
	}
}

template<typename keyType>
void batree<keyType>::getPath(stack<PNODE>& path, PNODE r, PNODE& ans, bool& tag, keyType k) {
	//cout << "In getPath\n";
	while (r && !company(path, r, ans)) {
		path.push(r);
		r = r->parent;
		if (r && !tag)
			getNode(r, ans, tag, k);
	}
	//cout << "Mid getPath\n";
	if (!path.empty()) {
		if (!tag && r) {
			r = r->parent;
			if (r)
				r->curLock.lock();	//lock
			while (!tag && r) {
				getNode(r, ans, tag, k);
				if (!tag) {
					if (r->parent)
						r->parent->curLock.lock();	//lock
					r->curLock.unlock();	//unlock
				}
				r = r->parent;
			}
		}
	}
	//cout << "Out getPaht!!!\n";
}

template<typename keyType>
void batree<keyType>::doIt(stack<PNODE>& path, PNODE& r, PNODE& tmp, PNODE ans) {
	//cout << "In doIt\n";
	r = NULL, tmp = NULL;
	while (!path.empty()) {
		r = path.top();
		path.pop();
		tmp = r->parent;

		//cout << "\tMERGE: ";
		//if (tmp)
		//	tmp->show();
		//cout << endl;

		if (tmp) {
			if (r->getN() == MIN_DEGREE) {
				int i = r->getI();
				if (i > 0)
					merge(path, tmp, i - 1, tmp->getC(i - 1), r);
				else
					merge(path, tmp, i, r, tmp->getC(i + 1));
			}
			if (tmp != ans && !path.empty())
				tmp->curLock.unlock();//unlock
		}
	}
	//cout << "Out doIt!!!\n";
}

template<typename keyType>
void batree<keyType>::merge(stack<PNODE>& path, PNODE x, int i, PNODE y, PNODE z) {
	//i: the index of key in x, y: left child of x, z: right child of x
	int j = DEGREE, basis = 0, len = MAX_DEGREE;
	if (y->getL()) {
		j--;
		basis++;
		len = y->getN() + z->getN();
	}
	y->setN(len);
	for (; j < len; j++)
		y->setK(j, z->getK(j - DEGREE + basis));
	if (!y->getL()) {
		y->setK(MIN_DEGREE, x->getK(i));
		for (j = DEGREE; j < len + 1; j++) {
			y->setC(j, z->getC(j - DEGREE));
			y->getC(j)->setP(y);
		}
	}
	for (j = i + 1; j < x->getN(); j++) {
		x->setK(j - 1, x->getK(j));
		x->setC(j, x->getC(j + 1));
	}
	x->setN(x->getN() - 1);
	if (!z->curLock.try_lock())
		y->curLock.lock();	//lock
	z->curLock.unlock();	//unlock
	delete z;
	//show();//test
}

template<typename keyType>
void* batree<keyType>::mergePath(PNODE r, keyType k, int x, int y) {
	//cout << "In mergePath\n";
	stack<PNODE>				path;		//store the path from BOTTOM up to TOP
	bool tag = false;
	PNODE ans = NULL;
	PNODE tmp = (PNODE)findLeaf(r, k);
	if (tmp != r) {
		//cout << "Leaf\n";
		tmp->curLock.lock();	//lock
		getNode(r, ans, tag, k);
		if (!company(path, tmp, ans))
			path.push(tmp);
		else {
			//cout << "Up to top\n";
			if (!tag) {
				r = r->parent;
				if (r)
					r->curLock.lock();	//lock
			}
			//else {
			//	cout << "\tANS: ";
			//	ans->show();
			//	cout << endl;
			//}
			while (!tag && r) {
				getNode(r, ans, tag, k);
				if (!tag) {
					if (r->parent)
						r->parent->curLock.lock();	//lock
					r->curLock.unlock();	//unlock
				}
				r = r->parent;
			}
			//cout << "Yeah?\n";
			queries[x][y].setC(ans);
			//cout << "Out mergePath!!!\n";
			return tmp;
		}
	}
	//else
	//	cout << "Inner\n";
	getPath(path, r, ans, tag, k);
	queries[x][y].setC(ans);
	doIt(path, r, tmp, ans);
	if (tmp) {
		//tmp->show();
		//cout << endl;
		//cout << "Out mergePath!!!\n";
		PNODE res = (PNODE)findLeaf(tmp, k);
		if (tmp != ans)
			tmp->curLock.unlock();	//unlock
		return res;
	}
	if (r) {
		//r->show();
		//cout << endl;
		//cout << "Out mergePath!!!\n";
		return r;
	}
	//cout << "Out mergePath!!!\n";
	return root;
}

template<typename keyType>
void batree<keyType>::del(int	x, int	y) {	//delete the k from root
	//cout << "In del\n";
	keyType k = queries[x][y].getK();
	if (queries[x][y].ans) {
		//cout << "Yeah?\n";	//test
		PNODE r = queries[x][y].lock();
		//cout << "-.0\n";

		//cout << "\t del " << k << "\n";	//test
		//cout << "Parent: ";	//test
		//r->show();	//test
		//cout << "\n";	//test

		r = (PNODE)mergePath(r, k, x, y);

		//cout << "\tCur: ";	//test
		//r->show();	//test
		//cout << "\n";	//test

		int i = 0;
		if (r->getC(0))//for the leaf node
			while (i < r->getN() && k >= r->getK(i)) i++;
		else//for the inner node
			while (i < r->getN() && k > r->getK(i)) i++;
		for (int j = i + 1; j < r->getN(); j++)
			r->setK(j - 1, r->getK(j));
		r->setN(r->getN() - 1);

		//cout << "\tWe deled: ";	//test
		//r->show();	//test
		//cout << "\n";	//test
		
		if (i == 0)
			delSet(k, r->getK(0), x, y);//reset the head - IMPERATIVE
		else {
			//cout << "No need!\n";
			PNODE tmp = queries[x][y].getC();
			if (tmp) {
				//cout << "\ttmp: ";
				//tmp->show();
				//cout << endl;
				tmp->curLock.unlock();
			}
		}
		r->curLock.unlock();	//unlock
	}
#ifdef TESTBTT
	show(x, y);
#endif
	//cout << "Out del!!!\n";
}

template<typename keyType>
void batree<keyType>::delSet(keyType k, keyType v, int x, int y) {	//reset value accoding to the head in inner node
	//cout << "In delSet\n";
	PNODE r = queries[x][y].getC();
	if (r) {
		int len = r->getN();
		int i = 0;
		while (i < len && k > r->getK(i)) i++;
		r->setK(i, v);
		//cout << "\tdleSet: ";
		//r->show();
		//cout << endl;
		r->curLock.unlock();	//unlock
	}
	//cout << "Out delSet!!!\n";
}

#ifdef SYNC
template<typename keyType>
void* batree<keyType>::mergePathSync(PNODE r, keyType k, QUERY& t) {
	stack<PNODE>				path;		//store the path from BOTTOM up to TOP
	bool tag = false;
	PNODE ans = NULL;
	PNODE tmp = (PNODE)findLeaf(r, k);
	if (tmp != r) {
		tmp->curLock.lock();	//lock
		getNode(r, ans, tag, k);
		if (!company(path, tmp, ans))
			path.push(tmp);
		else {
			if (!tag) {
				r = r->parent;
				if (r)
					r->curLock.lock();	//lock
			}
			while (!tag && r) {
				getNode(r, ans, tag, k);
				if (!tag) {
					if (r->parent)
						r->parent->curLock.lock();	//lock
					r->curLock.unlock();	//unlock
				}
				r = r->parent;
			}
			t.setC(ans);
			return tmp;
		}
	}
	getPath(path, r, ans, tag, k);
	t.setC(ans);
	doIt(path, r, tmp, ans);
	if (tmp) {
		PNODE res = (PNODE)findLeaf(tmp, k);
		if (tmp != ans)
			tmp->curLock.unlock();	//unlock
		return res;
	}
	if (r) {
		return r;
	}
	return root;
}

template<typename keyType>
void batree<keyType>::delSync(QUERY& t) {	//delete the k from root
	keyType k = t.getK();
	if (t.ans) {
		PNODE r = t.lock();
		r = (PNODE)mergePathSync(r, k, t);
		int i = 0;
		if (r->getC(0))//for the leaf node
			while (i < r->getN() && k >= r->getK(i)) i++;
		else//for the inner node
			while (i < r->getN() && k > r->getK(i)) i++;
		for (int j = i + 1; j < r->getN(); j++)
			r->setK(j - 1, r->getK(j));
		r->setN(r->getN() - 1);
		if (i == 0)
			delSetSync(k, r->getK(0), t);//reset the head - IMPERATIVE
		else {
			PNODE tmp = t.getC();
			if (tmp) {
				tmp->curLock.unlock();
			}
		}
		r->curLock.unlock();	//unlock
	}
}

template<typename keyType>
void batree<keyType>::delSetSync(keyType k, keyType v, QUERY& t) {	//reset value accoding to the head in inner node
	PNODE r = t.getC();
	if (r) {
		int len = r->getN();
		int i = 0;
		while (i < len && k > r->getK(i)) i++;
		r->setK(i, v);
		r->curLock.unlock();	//unlock
	}
}
#endif

template<typename keyType>
void batree<keyType>::shiftLTR(PNODE x, int i, PNODE y, PNODE z) {//x's right child y borrows a key and a child from x's left child of z
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
		z->getC(0)->setP(z);
	}
	z->setN(z->getN() + 1);
	y->setN(y->getN() - 1);
}

template<typename keyType>
void batree<keyType>::shiftRTL(PNODE x, int i, PNODE y, PNODE z) {//...
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
	if (!z->getL()) {
		y->setC(n + 1, z->getC(0));
		y->getC(n + 1)->setP(y);
		for (int k = 1; k <= z->getN(); k++)
			z->setC(k - 1, z->getC(k));
	}
	y->setN(n + 1);
	z->setN(z->getN() - 1);
}

template<typename keyType>
void batree<keyType>::doShow(ofstream& out, PNODE tmp, int d) {	//show the nodes in the order of floor
	for (int i = 0; i < d; i++) out << "   ";
	if (d) out << "->";
	out << "(" << tmp->getN() << ": ";
	for (int i = 0; i < tmp->getN(); i++)
		out << tmp->getK(i) << " ";
	if (tmp->leaf)
		out << "~";
	out << ")" << endl;
	if (tmp->getC(0))
		for (int i = 0; i <= tmp->getN(); i++)
			doShow(out, tmp->getC(i), d + 1);
}

template<typename keyType>
void batree<keyType>::show(int y) {//API for showing the btrees
	if (root) {
		ofstream out(treeFileName, ios::app);
		if (y >= 0) {
			for (int i = 0; i < THREAD_NUM; i++) {
				out << queries[i][y];
				out << "\n";
			}
		}
		doShow(out, root, 0);
		out.close();
	}
}

template<typename keyType>
void batree<keyType>::show(int	x, int	y) {//API for showing the btrees
	if (root) {
		ofstream out(treeFileName, ios::app);
		out << queries[x][y] << '\n';
		doShow(out, root, 0);
		out << "\n";
		out.close();
	}
}

template<typename keyType>
void batree<keyType>::testParent(keyType n) {//API for showing the btrees
	ofstream out(treeFileName, ios::app);
	PNODE tmp = (PNODE)findLeaf(root, n);
	while (tmp) {
		int i, n = tmp->getN();
		for (i = 0; i < n; i++)
			out << tmp->getK(i) << " ";
		out << endl;
		tmp = tmp->getP();
	}
	out.close();
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
	doClear(root);
}
