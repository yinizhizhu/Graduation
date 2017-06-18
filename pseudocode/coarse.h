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
		keyType	key[MAX_DEGREE];
		node*	child[MAX_DEGREE + 1];
		node(int k_n = 0, bool l = false) {
			for (int i = MAX_DEGREE; i >= 0; i--)
				child[i] = NULL;
			leaf = l;
			key_n = k_n;
		}
		int		getN() { return key_n; }
		bool	getL() { return leaf; }
		keyType	getK(int i) { return key[i]; }
		node*	getC(int i) { return child[i]; }
		void	setN(int n) { key_n = n; }
		void	setL(bool b) { leaf = b; }
		void	setK(int i, int n) { key[i] = n; }
		void	setC(int i, node* t) { child[i] = t; }
	} NODE, *PNODE;
	PNODE				root;
	mutex				proModify;
	vector<thread>		threads;

public:
	batree();
	~batree();
	void	insert(int	x, int	y);
	void	split(PNODE	x, int	i);
	void	insertNon(PNODE	x, keyType	k);
	void	del(int	x, int	y);
	void	delNon(PNODE	x, keyType	k);
	void	delSet(keyType	k, keyType	v);
	void	merge(PNODE	x, int	i, PNODE	y, PNODE	z);
	void	shiftRTL(PNODE	x, int	i, PNODE	y, PNODE	z);
	void	shiftLTR(PNODE	x, int	i, PNODE	y, PNODE	z);
	void	search(int	x, int	y);
	void*	findLeaf(keyType	k);
	void	getTree();
	void	fastRandom();
	void	doClear(PNODE root);
	void	clear();
};
