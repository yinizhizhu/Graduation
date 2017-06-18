template<typename keyType>
class batree {
private:
	PNODE	root;
	int		deep;
	//	< PNODE, PINFO >
	unordered_map<void*, void*>		list;
	//	< PNODE parent, PMODIFY child>
	unordered_map<void*, void*>		modifyList;
	vector<thread>					threads;
	vector<keyType>					orphanedKey;
public:
	batree();
	~batree();
	void	palm();
	bool	search(keyType	k);
	void*	findLeaf(keyType	k);
	void	modifyNode(infoIter	inf, INDEX	p);
	void	handleRoot(infoIter	ope);
	void	getBuffer(vector<PNODE>&	child, infoIter	inf,
		vector<keyType>&	buffer, INDEX	p);
	void	soft(vector<PNODE>&	child, vector<keyType>&	buffer);
	void	collect(PNODE head);
	void	merge(keyType	key, PNODE	y, PNODE	z);
	void	shiftRTL(keyType&	key, int	n, PNODE	y, PNODE	z);
	void	shiftLTR(keyType&	key, int	n, PNODE	y, PNODE	z);
	void	pushModify(PNODE	parent, PMODIFY	child, PMODIFY	moveChild);
	void	doClear(PNODE	root);
	void	clear();
};
#endif