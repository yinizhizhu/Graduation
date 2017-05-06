#include <iostream>
#include <stack>
#include <queue>

using namespace std;

typedef struct node
{
	int value;
	struct node* lChild;
	struct node* rChild;
	node(int a) :value(a), lChild(NULL), rChild(NULL){}
} node;

void initT(node* root)	//get the full binary tree
{
	int i, len;
	int counter = 1, level = 2;
	node* newOne, *tmp = NULL;
	queue<node*> container;
	container.push(root);
	while (level > 0)
	{
		len = container.size();
		for (i = 0; i < len; i++)
		{
			tmp = container.front();
			container.pop();

			newOne = new node(++counter);
			tmp->lChild = newOne;
			container.push(newOne);
			newOne = new node(++counter);
			tmp->rChild = newOne;
			container.push(newOne);
		}
		level--;
	}
	return;
}

node *build(vector<int>& A, int headA, int tailA, vector<int>& B, int headB, int tailB)
{
	int i, len, tmp = A[headA];
	for (i = headB; i <= tailB; i++)
		if (B[i] == tmp)
			break;
	
	len = i - headB;
	node *newOne = new node(tmp);
	if (len)
		newOne->lChild = build(A, headA + 1, headA + len, B, headB, i - 1);
	len = tailB - i;
	if (len)
		newOne->rChild = build(A, tailA - len + 1, tailA, B, i + 1, tailB);
	return newOne;
}

void initT2(node *&root, vector<int>& A, vector<int>& B)//A - preorder, B - inorder
{
	root = build(A, 0, A.size() - 1, B, 0, B.size() - 1);
	return;
}

void preO(node* T)
{
	if (T != NULL)
	{
		cout << T->value << " ";
		preO(T->lChild);
		preO(T->rChild);
	}
}

void inO(node* T)
{
	if (T != NULL)
	{
		inO(T->lChild);
		cout << T->value << " ";
		inO(T->rChild);
	}
}

void postO(node* T)
{
	if (T != NULL)
	{
		postO(T->lChild);
		postO(T->rChild);
		cout << T->value << " ";
	}
}

void inO2(node* root)
{
	node* T = root;
	stack<node*> s;
	while (T || !s.empty())
	{
		if (T)
		{
			s.push(T);
			T = T->lChild;
		}
		else
		{
			T = s.top();	//Remove the node visited
			s.pop();
			cout << T->value << " ";
			T = T->rChild;
		}
	}
	return;
}

void postO2(node* root)
{
	node *p = root, *r = NULL;
	stack<node*> s;
	while (p || !s.empty())
	{
		if (p)
		{
			s.push(p);
			p = p->lChild;
		}
		else
		{
			p = s.top();
			if (p->rChild && p->rChild != r)
				p = p->rChild;
			}
			else
			{
				p = s.top();
				s.pop();
				cout << p->value << " ";
				r = p;
				p = NULL;
			}
		}
	}
	return;
}

void levelO(node* root)
{
	int i, len;
	node* tmp;
	queue<node*> container;
	container.push(root);
	while (!container.empty())
	{
		len = container.size();
		for (i = 0; i < len; i++)
		{
			tmp = container.front();
			container.pop();

			cout << tmp->value << " ";

			if (tmp->lChild)
				container.push(tmp->lChild);
			if (tmp->rChild)
				container.push(tmp->rChild);
		}
	}
	return;
}

void exchangeLR(node* root)
{
	if (root)
	{
		node* tmp = NULL;
		tmp = root->lChild;
		root->lChild = root->rChild;
		root->rChild = tmp;
		exchangeLR(root->lChild);
		exchangeLR(root->rChild);
	}
	return;
}

void cTwoDegreeNode(node* root, int& counter)
{
	if (root)
	{
		if (root->lChild && root->rChild)
			counter++;
		cTwoDegreeNode(root->lChild, counter);
		cTwoDegreeNode(root->rChild, counter);
	}
	return;
}

void getKthInO(node* root, int& move, bool& tag)
{
	if (root && tag)
	{
		cout << root->value << " ";
		move--;
		if (move == 0)
		{
			move = root->value;
			tag = false;
		}
		getKthInO(root->lChild, move, tag);
		getKthInO(root->rChild, move, tag);
	}
	return;
}

int main()
{
	//node* root = new node(1);
	//initT(root);

	vector<int> A, B;
	for (int i = 1; i < 10; i++)
		A.push_back(i);
	B.push_back(2);
	B.push_back(3);
	B.push_back(1);
	B.push_back(5);
	B.push_back(4);
	B.push_back(7);
	B.push_back(8);
	B.push_back(6);
	B.push_back(9);

	node *root = NULL;
	initT2(root, A, B);

	preO(root);
	cout << endl;

	int move = 7;
	bool tag = true;
	getKthInO(root, move, tag);
	cout << endl;
	cout << move << endl;

	//postO(root);
	//cout << endl;

	//postO2(root);
	//cout << endl;
	return 0;
}