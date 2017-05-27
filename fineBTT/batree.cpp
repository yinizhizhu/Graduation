#include "batree.h"

template<typename keyType>
batree<keyType>::batree() {	//initial
	root = new NODE(NULL, 0, true);
	queries.resize(THREAD_NUM);

	ofstream out(TREE_FILE_NAME);
	out.close();
	ofstream out2(QUERY_FILE_NAME);
	out2.close();
}

template<typename keyType>
batree<keyType>::~batree() {	//free the sources
	clear();
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
	cout << "In fastRandom\n";
	int i, j;
#ifdef TESTBTT
	for (i = 0; i < THREAD_NUM; i++)
		for (j = 0; j < EACH_NUM; j++) {
			queries[i].push_back(QUERY(INS_STEP, rand() % 1000));
			search(i, j);
			insert(i, j);
			testParent(900);
		}
	queries[0][0].key = 25;
	search(0, 0);
	insert(0, 0);
	testParent(900);

	int n;
	cout << "Input the number of test cases: ";
	cin >> n;
	queries[0][0].type = DEL_STEP;
	for (; n > 0; n--) {
		cin >> i;
		cout << "Delete: " << i << endl;
		queries[0][0].key = i;
		search(0, 0);
		del(0, 0);
	}
#else
	for (i = 0; i < THREAD_NUM; i++)
		for (j = 0; j < EACH_NUM; j++)
			queries[i].push_back(QUERY((STEP_TYPE)(rand() % 3), rand() % 39));
	outputQuery(QUERY_FILE_NAME);
	vector<int> index;
	for (j = 0; j < EACH_NUM; j++) {
		cout << "Mid fastRandom:" << j << "\n";
		for (i = 0; i < THREAD_NUM; i++) {
			threads.push_back(thread(&batree<keyType>::search, this, i, j));
			switch (queries[i][j].type) {
			case FIN_STEP:
				break;
			case INS_STEP:
			case DEL_STEP:
				index.push_back(i);
				break;
			default:
				cout << "What's wrong!\n";
				break;
			}
		}
		for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
		threads.clear();
		cout << "Finish the search " << j << '\n';
		i = index.size() - 1;
		if (i >= 0) {
			for (; i >= 0; i--) {
				if (queries[index[i]][j].type == INS_STEP)
					threads.push_back(thread(&batree<keyType>::insert, this, index[i], j));
				else
					threads.push_back(thread(&batree<keyType>::del, this, index[i], j));
			}
			//mem_fn(&thread::join);
			for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
			threads.clear();
			index.clear();
		}
		cout << "Out Mid " << j << "!!!\n";
	}
	outputQuery(QRESULT_FILE_NAME);
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
			//cout << "Yeah! Get " << k << "\t";
			queries[x][y].setA(true);
			//cout << queries[x][y].ans << '\n';
			if (queries[x][y].type == FIN_STEP || queries[x][y].type == INS_STEP)
				return;
		}
		if (r->getL())
			break;
		r = r->getC(i);
	}
	if (queries[x][y].type != FIN_STEP)
		queries[x][y].setC(r);
	//cout << "Out search!!!\n";
}

template<typename keyType>
void batree<keyType>::split(PNODE x, int i) {	//split the child whose index is i of node x
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
	//cout << endl;//test
}

template<typename keyType>
void* batree<keyType>::splitPath(PNODE r, keyType k) {
	//r->curLock.lock();	//lock the current node in the path from bottom up to top
	while (r && r->getN() == MAX_DEGREE) {
		path.push(r);
		r = r->parent;
		//r->curLock.lock();	//lock the current node in the path from bottom up to top
	}
	if (path.empty())
		return r;
	PNODE top = path.top();
	path.pop();
	if (top->parent) {
		//top->parent->curLock.lock();		//lock
		split(top->parent, top->getI());
		//top->parent->curLock.unlock();		//unlock
	}
	else {
		PNODE tmp = new NODE();
		//tmp->curLock.lock();	//lock
		tmp->setC(0, root);
		root->setP(tmp);
		root = tmp;
		split(root, 0);
		//tmp->curLock.unlock		//unlock
	}
	while (!path.empty()) {
		top = path.top();
		path.pop();
		split(top->parent, top->getI());
		//top->parent->curLock.unlock();	//unlock
	}
	if (path.empty()) {
		r = (PNODE)findLeaf(top->parent, k);
		//r->curLock.lock();	//lock
	}
	//top->curLock.unlock();	//unlock
	return r;
}

template<typename keyType>
void batree<keyType>::insert(int	x, int	y) {	//insert the k into root
	//Before inserting, we split the full node
	lock_guard<mutex> guard(proModify);
	keyType k = queries[x][y].getK();
	if (queries[x][y].ans)
		return;
	PNODE r = queries[x][y].getC();
	r = (PNODE)splitPath(r, k);
	int i = r->getN() - 1;
	while (i >= 0 && k < r->getK(i)) {
		r->setK(i + 1, r->getK(i));
		i--;
	}
	r->setK(i + 1, k);
	r->setN(r->getN() + 1);
	//r->curLock.unlock();	//lock
	show(x, y);
	//cout << "Out insert!!!\n";
}

template<typename keyType>
bool batree<keyType>::company(PNODE r) {
	if (r->parent) {	//Non root
		if (r->getN() > MIN_DEGREE)
			return true;
		PNODE x = r->parent;
		int i = r->getI();
		int len = x->getN();
		if (i > 0 && x->getC(i - 1)->getN() > MIN_DEGREE) {
			shiftLTR(x, i - 1, x->getC(i - 1), r);
			return true;
		}
		else if (i < len && x->getC(i + 1)->getN() > MIN_DEGREE) {
			shiftRTL(x, i, r, x->getC(i + 1));
			return true;
		}
		return false;
	}
	//for the root
	if (r->getN() == 1 && !r->getL()) {
		merge(r, 0, r->getC(0), r->getC(1));
		root = r->getC(0);
		root->setP(NULL);
		delete r;
	}
	return true;
}

template<typename keyType>
void batree<keyType>::merge(PNODE x, int i, PNODE y, PNODE z) {
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
	delete z;
	//show();//test
}

template<typename keyType>
void batree<keyType>::mergePath(PNODE r) {
	while (r && !company(r)) {
		path.push(r);
		r = r->parent;
	}
	if (path.empty())
		return;
	PNODE top = NULL;
	while (!path.empty()) {
		top = path.top();
		path.pop();
		if (top->getN() == MIN_DEGREE) {
			PNODE x = top->parent;
			int i = top->getI();
			if (i > 0)
				merge(x, i - 1, x->getC(i - 1), top);
			else
				merge(x, i, top, x->getC(i + 1));
		}
	}
}

template<typename keyType>
void batree<keyType>::del(int	x, int	y) {	//delete the k from root
	//cout << "In del\n";
	lock_guard<mutex> guard(proModify);
	keyType k = queries[x][y].getK();
	if (queries[x][y].ans) {
		PNODE r = queries[x][y].getC();
		r->show();
		cout << endl;
		mergePath(r);
		r = (PNODE)findLeaf(root, k);
		int i = 0;
		if (r->getC(0))//for the leaf node
			while (i < r->getN() && k >= r->getK(i)) i++;
		else//for the inner node
			while (i < r->getN() && k > r->getK(i)) i++;
		for (int j = i + 1; j < r->getN(); j++)
			r->setK(j - 1, r->getK(j));
		r->setN(r->getN() - 1);
		if (i == 0) delSet(k, r->getK(0));//reset the head - IMPERATIVE
	}
	show(x, y);
	//cout << "Out del!!!\n";
}

template<typename keyType>
void batree<keyType>::delSet(keyType k, keyType v) {	//reset value accoding to the head in inner node
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
	y->setC(n + 1, z->getC(0));
	if (!z->getL())
		for (int k = 1; k <= z->getN(); k++)
			z->setC(k - 1, z->getC(k));
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
void batree<keyType>::show() {//API for showing the btrees
	if (root) {
		ofstream out(TREE_FILE_NAME, ios::app);
		doShow(out, root, 0);
		out.close();
	}
}

template<typename keyType>
void batree<keyType>::show(int	x, int	y) {//API for showing the btrees
	if (root) {
		ofstream out(TREE_FILE_NAME, ios::app);
		out << queries[x][y] << '\n';
		doShow(out, root, 0);
		out << "\n";
		out.close();
	}
}

template<typename keyType>
void batree<keyType>::testParent(keyType n) {//API for showing the btrees
	ofstream out(TREE_FILE_NAME, ios::app);
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
