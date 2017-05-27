#include "batree.h"

template<typename keyType>
batree<keyType>::batree() {	//initial
	root = new node(0, true);
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
	PNODE move = root;
	move->setL(false);
	move->setN(3);
	move->setK(0, 6);
	move->setK(1, 12);
	move->setK(2, 18);
	for (int i = 0, j = 2, k = 0; i <= 3; i++, j += 6) {
		PNODE tmp = new node(2, false);
		tmp->setK(0, j);
		tmp->setK(1, j + 2);
		for (int p = 0; p < 3; p++) {
			int n = 2;
			if (k == 22) n = 5;
			PNODE one = new node(n, true);
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
	if (queries[x][y].type != FIN_STEP)
		queries[x][y].setC(r);
	//cout << "Out search!!!\n";
}

template<typename keyType>
void batree<keyType>::split(PNODE x, int i) {	//split the child whose index is i of node x
	//x - current node, i - the index of node which will be splited
	int len = MIN_DEGREE, basis = 0;//for the iner node
	PNODE z = new node(), y = x->getC(i);
	z->setL(y->getL());
	if (z->getL()) {//for the leaf node
		len++;
		basis = -1;
	}
	z->setN(len);
	for (int j = 0; j < len; j++)
		z->setK(j, y->getK(j + DEGREE + basis));
	if (y->leaf == false)
		for (int j = 0; j <= len; j++)
			z->setC(j, y->getC(j + DEGREE + basis));
	y->setN(MIN_DEGREE);
	//y->show();//test
	//z->show();//test
	for (int j = x->getN(); j > i; j--)
		x->setC(j + 1, x->getC(j));
	x->setC(i + 1, z);
	for (int j = x->getN() - 1; j >= i; j--)
		x->setK(j + 1, x->getK(j));
	x->setK(i, y->getK(MIN_DEGREE));
	x->setN(x->getN() + 1);
	//x->show(0);//test
	//cout << endl;//test
}

template<typename keyType>
void batree<keyType>::insertNon(PNODE x, keyType k) {	//insert the k into the subtree whose root is node x
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
		if (x->getC(i)->getN() == MAX_DEGREE) {
			split(x, i);
			if (k > x->getK(i)) i++;
		}
		insertNon(x->getC(i), k);
	}
}

template<typename keyType>
void batree<keyType>::insert(int	x, int	y) {	//insert the k into root
	//Before inserting, we split the full node
	//cout << "In insert\n";
	lock_guard<mutex> guard(proModify);
	ofstream out(QUERY_FILE_NAME, ios::app);
	keyType k = queries[x][y].getK();
	out << queries[x][y];
	out << queries[x][y].getC();
	search(x, y);
	if (queries[x][y].ans) {
		out << '\n';
		out.close();
		return;
	}
	PNODE r = root;
	if (MAX_DEGREE == r->getN()) {
		PNODE s = new node();
		root = s;
		s->setN(0);
		s->setL(false);
		s->setC(0, r);
		split(s, 0);
		insertNon(s, k);
	}
	else insertNon(r, k);
	out << "-> ";
	out << (PNODE)findLeaf(k) << '\n';
	out.close();
	show(x, y);
	//cout << "Out insert!!!\n";
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
		for (j = DEGREE; j < len + 1; j++)
			y->setC(j, z->getC(j - DEGREE));
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
void batree<keyType>::del(int	x, int	y) {	//delete the k from root
	//cout << "In del\n";
	lock_guard<mutex> guard(proModify);
	ofstream out(QUERY_FILE_NAME, ios::app);
	keyType k = queries[x][y].getK();
	out << queries[x][y];
	out << queries[x][y].getC() << '\n';
	out.close();
	search(x, y);
	if (queries[x][y].ans) {
		PNODE r = root;
		if (r->getN() == 1 && !r->getL()) {
			PNODE y = root->getC(0);
			PNODE z = root->getC(1);
			int n = y->getN() + z->getN();
			if ((!y->getL() && n <= MAX_DEGREE - 1) || (y->getL() && n <= MAX_DEGREE)) {
				merge(r, 0, y, z);
				root = y;
				delete r;
				delNon(y, k);
			}
			else delNon(r, k);
		}
		else delNon(r, k);
	}
	show(x, y);
	//cout << "Out del!!!\n";
}

template<typename keyType>
void batree<keyType>::delNon(PNODE x, keyType k) {
	int i = 0;
	if (x->getC(0))//for the leaf node
		while (i < x->getN() && k >= x->getK(i)) i++;
	else//for the inner node
		while (i < x->getN() && k > x->getK(i)) i++;
	if (x->getL()) {//Reach the leaf node
		if (k == x->getK(i)) {
			for (int j = i + 1; j < x->getN(); j++)
				x->setK(j - 1, x->getK(j));
			x->setN(x->getN() - 1);
			if (i == 0) delSet(k, x->getK(0));//reset the head - IMPERATIVE
		}
		//else cout << "The key does not exist!" << endl;
		return;
	}
	// the iner node
	PNODE z = NULL, p = NULL, y = x->getC(i);
	if (i < x->getN()) z = x->getC(i + 1);
	if (i > 0) p = x->getC(i - 1);
	if (y->getN() == MIN_DEGREE) {
		if (i > 0 && p->getN() >= DEGREE)//Get: try the left side
			shiftLTR(x, i - 1, p, y);
		else if (i < x->getN() && z->getN() >= DEGREE)//Get: try the right side
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
void batree<keyType>::doShow(ofstream& out, PNODE root, int d) {	//show the nodes in the order of floor
	PNODE tmp = root;
	for (int i = 0; i < d; i++) out << "   ";
	if (d) out << "->";
	out << "(" << tmp->getN() << ": ";
	for (int i = 0; i < tmp->getN(); i++)
		out << tmp->getK(i) << " ";
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
		out.close();
	}
}

template<typename keyType>
void batree<keyType>::doClear(PNODE t) {	//show the nodes in the order of floor
	if (t->getC(0))
		for (int i = 0; i <= t->getN(); i++)
			doClear(t->getC(i));
	delete t;
}

template<typename keyType>
void batree<keyType>::clear() {//API for free the sources we apply
	doClear(root);
}
