#include <iostream>
#include <stack>

using namespace std;

template<class Type>
class AVLTree;

//AVL node
template<class Type>
class AVLNode
{
	friend AVLTree<Type>;
public:
	AVLNode(Type d = Type(), AVLNode<Type>* left = nullptr, AVLNode<Type>* right = nullptr)
		: data(d), leftChild(left), rightChild(right), bf(0)
	{}

	~AVLNode()
	{}

private:
	Type			data;
	AVLNode<Type>*	leftChild;
	AVLNode<Type>*	rightChild;
	int				bf;  //平衡因子
};

template<class Type>
class AVLTree
{
public:
	AVLTree() : root(nullptr)
	{}
public:
	bool Insert(const Type& x)
	{
		return Insert(root, x);
	}
	bool Remove(const Type& key)
	{
		return Remove(root, key);
	}
protected:
	bool Insert(AVLNode<Type>*& t, const Type& x);
	bool Remove(AVLNode<Type>*& t, const Type& key);
	void Rotate_R(AVLNode<Type>*& ptr); //ptr指向不平衡的节点
	void Rotate_L(AVLNode<Type>*& ptr);
	void Rotate_LR(AVLNode<Type>*& ptr);
	void Rotate_RL(AVLNode<Type>*& ptr);
private:
	AVLNode<Type>* root;
};

//1.调整平衡 2.更改bf
template<class Type>
void AVLTree<Type>::Rotate_R(AVLNode<Type>*& ptr)
{
	AVLNode<Type>* subR = ptr; //旋转之后的右子树
	ptr = subR->leftChild;     //ptr为最后的新根
	subR->leftChild = ptr->rightChild; //连接旋转后的subR之前先将ptr的右树连接到subR的左树上
	ptr->rightChild = subR;

	ptr->bf = subR->bf = 0;		//更改bf
}
template<class Type>
void AVLTree<Type>::Rotate_L(AVLNode<Type>*& ptr)
{
	AVLNode<Type>* subL = ptr; //旋转之后的左子树
	ptr = subL->rightChild;
	subL->rightChild = ptr->leftChild;
	ptr->leftChild = subL;

	ptr->bf = subL->bf = 0;
}
template<class Type>
void AVLTree<Type>::Rotate_LR(AVLNode<Type>*& ptr)
{
	AVLNode<Type>* subL = ptr->leftChild;
	AVLNode<Type>* subR = ptr;
	ptr = subL->rightChild;

	//先左旋转
	subL->rightChild = ptr->leftChild;
	ptr->leftChild = subL;

	//更新subL->bf       //画图分情况推导
	if (ptr->bf <= 0)
		subL->bf = 0;
	else
		subL->bf = -1;

	//后右旋转
	subR->leftChild = ptr->rightChild;
	ptr->rightChild = subR;

	//更新subR->bf
	if (ptr->bf >= 0)
		subR->bf = 0;
	else
		subR->bf = 1;

	ptr->bf = 0;
}
template<class Type>
void AVLTree<Type>::Rotate_RL(AVLNode<Type>*& ptr)
{
	AVLNode<Type>* subL = ptr;
	AVLNode<Type>* subR = ptr->rightChild;;
	ptr = subR->leftChild;

	//先右旋转
	subR->leftChild = ptr->rightChild;
	ptr->rightChild = subR;

	//更新subR->bf
	if (ptr->bf >= 0)
		subR->bf = 0;
	else
		subR->bf = 1;

	//后左旋转
	subL->rightChild = ptr->leftChild;
	ptr->leftChild = subL;

	//更新subL->bf
	if (ptr->bf == 1)
		subL->bf = -1;
	else
		subL->bf = 0;

	ptr->bf = 0;
}

template<class Type>
bool AVLTree<Type>::Insert(AVLNode<Type>*& t, const Type& x)
{
	// 1.按照BST的规则插入节点
	AVLNode<Type>* pr = nullptr; //要插入节点的父节点
	AVLNode<Type>* p = t;  //插入第一个数据时t指向空

	stack<AVLNode<Type>*> st;

	while (p != nullptr) //用于查找x的插入位置
	{
		if (x == p->data) //不插入重复数据
			return false;

		pr = p; //追踪要插入节点的父节点
		st.push(pr);  //保留经过的节点

		if (x < p->data)
			p = p->leftChild;
		else
			p = p->rightChild;
	}
	p = new AVLNode<Type>(x);
	if (pr == nullptr)
	{
		t = p;
		return true;
	}

	//链接新建节点
	if (x < pr->data)
		pr->leftChild = p;
	else
		pr->rightChild = p;

	// 2.如果发生不平衡，需要调整平衡
	while (!st.empty())
	{
		pr = st.top();
		st.pop();

		if (p == pr->leftChild) 
			pr->bf--;  //如果在pr的左树插入，pr的bf减1（bf=r_h - l_h）;
		else
			pr->bf++;

		if (pr->bf == 0)
			break;  //如果插入节点后pr->bf==0,树依旧平衡，不需要再回溯上去调整bf
		if (pr->bf == 1 || pr->bf == -1) //|bf|=1
			p = pr;	//向上回溯
		else
		{
			// pr->|bf| == 2 发生不平衡，需要调整平衡
			if (p == pr->leftChild) //左分支
			{
				if (p->bf < 0)   /*   / -->  右单旋    */
					Rotate_R(pr);
				else			 /*   < -->  先左后右  */
					Rotate_LR(pr);
			}
			else   //右分支
			{  
				if (p->bf > 0)   /*   \ -->  左单旋    */
					Rotate_L(pr);
				else			 /*   > -->  先右后左  */
					Rotate_RL(pr);
			}
			break;
		}
	}

	//重新连接
	if (st.empty())
		t = pr;
	else
	{
		AVLNode<Type>* ppr = st.top();  //父节点的父节点
		if (ppr->data > pr->data)
			ppr->leftChild = pr;
		else
			ppr->rightChild = pr;
	}
	return true;
}

template<class Type>
bool AVLTree<Type>::Remove(AVLNode<Type>*& t, const Type& key)
{
	// 1.按照BST的规则找到节点并删除

	// 2.删除节点后，若发生不平衡，需要重新调整并更新bf
}

int main()
{
	int ar[] = { 16,3,7,11,9,26,18,14,15 };
	int n = sizeof(ar) / sizeof(ar[0]);

	AVLTree<int> avl;
	for (int i = 0; i < n; ++i)
	{
		avl.Insert(ar[i]);
	}

	return 0;
}