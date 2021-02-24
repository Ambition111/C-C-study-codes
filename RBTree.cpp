#include <iostream>
//#include <stack>

using namespace std;

typedef enum { RED, BLACK } Color_Type;


template<class Type>
class RBTree;

//红黑树：一头一脚黑、黑同红不连
template<class Type>
class RBTreeNode
{
	friend class RBTree<Type>;
	RBTreeNode(Type d = Type(), RBTreeNode<Type>* left = nullptr, RBTreeNode<Type>* right = nullptr)
		: data(d), leftChild(left), rightChild(right), parent(nullptr), color(RED)//每次都是以红色节点插入
	{}
	~RBTreeNode()
	{}
private:
	Type data;
	RBTreeNode<Type>* leftChild;
	RBTreeNode<Type>* rightChild;
	RBTreeNode<Type>* parent;   //指向父节点的指针
	Color_Type		  color;
};

template<class Type>
class RBTree
{
public:
	RBTree() : NIL(_BuyNode()), root(NIL)
	{
		NIL->leftChild = NIL->rightChild = NIL->parent = nullptr;
		NIL->color = BLACK;
		root = NIL;
	}
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
	bool Insert(RBTreeNode<Type>*& t, const Type& x);
	bool Remove(RBTreeNode<Type>*& t, const Type& key);
	void RightRotate(RBTreeNode<Type>*& t, RBTreeNode<Type>* p);
	void LeftRotate(RBTreeNode<Type>*& t, RBTreeNode<Type>* p);
	void Insert_Fixup(RBTreeNode<Type>*& t, RBTreeNode<Type>* x);
protected:
	RBTreeNode<Type>* _BuyNode(const Type& x = Type())
	{
		RBTreeNode<Type>* s = new RBTreeNode<Type>(x);
		s->leftChild = s->rightChild = s->parent = NIL;
		return s;
	}
private:
	RBTreeNode<Type>* root;
	RBTreeNode<Type>* NIL;   //所有的最后的叶子节点都指向这个真实的空节点
};

template<class Type>
bool RBTree<Type>::Insert(RBTreeNode<Type>*& t, const Type& x)
{
	// 1.按照BST的规则插入节点
	RBTreeNode<Type>* pr = NIL;  //父节点指针
	RBTreeNode<Type>* p = t;
	while (p != NIL)
	{
		if (x == p->data)
			return false;  //不能插入重复值

		pr = p;

		if (x < p->data)
			p = p->leftChild;
		else
			p = p->rightChild;
	}

	RBTreeNode<Type>* newNode = _BuyNode(x);

	//链接节点
	if (pr == NIL)  //创建根节点
	{
		t = newNode;     //如果pr为空，就让新节点为根
		t->parent = NIL; //根的父节点为空
	}
	else if (x < pr->data)
		pr->leftChild = newNode;
	else
		pr->rightChild = newNode;
	newNode->parent = pr;

	// 2.平衡调整(颜色)
	Insert_Fixup(t, newNode);

	return true;
}

template<class Type>
bool RBTree<Type>::Remove(RBTreeNode<Type>*& t, const Type& key)
{
	// 1.按照BST的规则删除节点

	// 2.调整平衡
}

template<class Type>
void RBTree<Type>::RightRotate(RBTreeNode<Type>*& t, RBTreeNode<Type>* p)
{
	RBTreeNode<Type>* q = p->leftChild;
	p->leftChild = q->rightChild;

	if (q->rightChild != NIL)
		q->rightChild->parent = p;
	q->parent = p->parent;

	if (p->parent == NIL)
		t = q;
	else if (p == p->parent->leftChild)
		p->parent->leftChild = q;
	else
		p->parent->rightChild = q;

	q->rightChild = p;
	p->parent = q;
}
template<class Type>
void RBTree<Type>::LeftRotate(RBTreeNode<Type>*& t, RBTreeNode<Type>* p)
{
	RBTreeNode<Type>* q = p->rightChild;
	p->rightChild = q->leftChild;
	if (q->leftChild != NIL)
		q->leftChild->parent = p;
	q->parent = p->parent;

	if (p->parent == NIL)
		t = q;
	else if (p == p->parent->leftChild)
		p->parent->leftChild = q;
	else
		p->parent->rightChild = q;

	q->leftChild = p;
	p->parent = q;

}

template<class Type>
void RBTree<Type>::Insert_Fixup(RBTreeNode<Type>*& t, RBTreeNode<Type>* x)
{
	while (x->parent->color == RED) //红红相连时，就需要调整
	{
		RBTreeNode<Type>* s;   //叔伯节点
		if (x->parent == x->parent->parent->leftChild) //左分支
		{
			s = x->parent->parent->rightChild;  //凡是不存在，都指向NIL(黑色)节点

			//状况三、四
			if (s->color == RED)
			{
				x->parent->color = BLACK;
				s->color = BLACK;
				x->parent->parent->color = RED;

				x = x->parent->parent;  //向上回溯
				continue;
			}
			//状况二
			if (x == x->parent->rightChild)
			{
				x = x->parent;
				LeftRotate(t, x); //通过一次左单旋转变为状况一的情况
			}
			//状况一
			x->parent->color = BLACK;		//更改P
			x->parent->parent->color = RED;	//更改G

			RightRotate(t, x->parent->parent);
		}
		else   //右分支
		{
			s = x->parent->parent->leftChild;

			//状况三、四
			if (s->color == RED)
			{
				x->parent->color = BLACK;
				s->color = BLACK;
				x->parent->parent->color = RED;

				x = x->parent->parent;
				continue;
			}
			//状况二
			if (x == x->parent->leftChild)
			{
				x = x->parent;
				RightRotate(t, x);
			}
			//状况一
			x->parent->color = BLACK;		//更改P
			x->parent->parent->color = RED;	//更改G
			LeftRotate(t, x->parent->parent);
		}
	}
	t->color = BLACK;
}

int main()
{
	int ar[] = { 10,7,8,15,5,6,11,13,12 };
	int n = sizeof(ar) / sizeof(ar[0]);
	RBTree<int> rb;
	for (int i = 0; i < n; ++i)
	{
		rb.Insert(ar[i]);
	}

	return 0;
}


/*
	X：新插入节点、P：父节点、S：叔伯节点、G：祖父节点、GG：曾祖父节点
	红黑树插入的4种状况：
	状况1：S为黑且X为外侧插入（形成一条直线），对此情况，先对P，G做一次单旋转，再更改P，G颜色。
	状况2：S为黑且X为内侧插入（形成一条折线），对此情况，必须先对P，X做一次单旋转并更改G，X颜色，再将结果对G做一次单旋转。
	状况3：S为红且X为外侧插入，对此情况，先对P、G做一次单旋转，并改变X的颜色，此时如果GG为黑，一切搞定，但如果GG为红，见状况4
			（也可以通过改变颜色（P、S、G）的方法调整状况3，如上面代码，但是要注意的是两种方法所调整的红黑树形状会不一样）
	状况4：S为红且X为外侧插入，对此情况，先对P、G做一次单旋转，并改变
*/




