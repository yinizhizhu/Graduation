template<typename keyType>
class batree {
private:
	enum STEP_TYPE {	//enumerate the type of each operation
		FIN_STEP,	//find
		INS_STEP,	//insert
		DEL_STEP,	//delete
		UPD_STEP	//update
	};
	typedef struct node {
		bool	leaf;
		int		key_n;
		mutex	curLock;
		keyType	key[MAX_DEGREE];
		node*	parent;
		node*	child[MAX_DEGREE + 1];
		node(node* p = NULL, int k_n = 0, bool l = false) {
			parent = p;
			for (int i = MAX_DEGREE; i >= 0; i--)
				child[i] = NULL;
			leaf = l;
			key_n = k_n;
		}
		int		getN() { return key_n; }
		int		getI() {
			if (parent) {
				node** pc = parent->child;
				int i = 0;
				for (;; i++) {
					if (pc[i] == this)
						return i;
				}
			}
			return 0;
		}
		bool	getL() { return leaf; }
		keyType	getK(int i) { return key[i]; }
		node*	getC(int i) { return child[i]; }
		node*	getP() { return parent; }
		void	setN(int n) { key_n = n; }
		void	setL(bool b) { leaf = b; }
		void	setK(int i, int n) { key[i] = n; }
		void	setC(int i, node* t) { child[i] = t; }
		void	setP(node* t) { parent = t; }
		void	show() {
			for (int i = 0; i < key_n; i++)
				cout << key[i] << " ";
		}
	} NODE, *PNODE;
	PNODE 			root;
	vector<thread>	threads;
public:
	batree();
	~batree();
	void	insert(int	x, int	y);
	void*	splitPath(PNODE	r, keyType	k);
	void	split(stack<PNODE>&	path, PNODE	x, int	i);
	void	del(int	x, int	y);
	void	delSet(keyType	k, keyType	v, int	x, int	y);
	void*	mergePath(PNODE	r, keyType	k, int	x, int	y);
	void	doIt(stack<PNODE>&	path, PNODE&	r, PNODE&	tmp, PNODE	ans);
	void	merge(stack<PNODE>&	path, PNODE	x, int	i, PNODE	y, PNODE	z);
	void	getPath(stack<PNODE>& path, PNODE r, PNODE& ans, bool& tag, keyType k);
	void	shiftRTL(PNODE	x, int	i, PNODE	y, PNODE	z);
	void	shiftLTR(PNODE	x, int	i, PNODE	y, PNODE	z);
	void	getTree();
	void	fastRandom();
	void	search(int	x, int	y);
	void*	findLeaf(PNODE	r, keyType	k);
	void	doClear(PNODE root);
	void	clear();
};
#endif
