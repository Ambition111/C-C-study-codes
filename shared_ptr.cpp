#include <iostream>
#include <memory>
//#include <vld.h>
#include <vector>
#include <thread>
#include <mutex>
#include <boost/smart_ptr.hpp>

using namespace boost;
using namespace std;


struct listNode
{
	int data;
	std::weak_ptr<listNode> prev;//使用weak_ptr是因为它不会增加prev和next的引用计数，避免造成循环引用
	std::weak_ptr<listNode> next;

	listNode(int d = 0) : data(d)
	{
		cout << "listNode::listNode()" << endl;
	}
	~listNode()
	{
		cout << "listNode::~listNode()" << endl;
	}
};

//定制删除器
void Del(listNode* pn)
{
	cout << "Delete pn" << endl;
	delete[] pn;
}
int main()
{
	listNode* pa = new listNode[5];
	
	//boost::shared_array<listNode> spa(pa);  //OK

	//C++11库里没有所谓的shared_array,但boost库里有，但若仍想利用std::shared_ptr管理数组空间，则需要定制删除器，其实就是删除函数
	//std::shared_ptr<listNode> spa(pa);	  //error
	std::shared_ptr<listNode> spa(pa,Del);	  //OK

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/*
//模拟实现shared_ptr
template<class T>
class my_shared_ptr
{
public:
	my_shared_ptr(T* _P = 0) : _Ptr(_P), _UseCount((size_t*)new int(1)), _Mutex(new mutex)
	{}
	my_shared_ptr(const my_shared_ptr<T>& p) : _Ptr(p._Ptr), _UseCount(p._UseCount), _Mutex(p._Mutex)
	{
		Increment();//增加引用计数
	}
	my_shared_ptr<T>& operator=(const my_shared_ptr<T>& p)
	{
		if (this != &p)
		{
			//释放资源
			Decrement();
			_Ptr = p._Ptr;
			_UseCount = p._UseCount;
			Increment();
		}
		return *this;
	}
	~my_shared_ptr()
	{
		Decrement();//减少引用计数
	}
public:
	T& operator*() const
	{
		return *_Ptr;
	}
	T* operator->() const
	{
		return _Ptr;
	}
public:
	size_t use_count() const
	{
		return *_UseCount;
	}
	bool unique() const
	{
		return (use_count() == 1);
	}
	void reset(T* _P = 0)
	{
		if (_Ptr != _P)
			Decrement();
		_Ptr = _P;
		_UseCount = (size_t*)new int(1);
	}
public:
	void Increment() 
	{
		_Mutex->lock();
		++*_UseCount;		//临界区
		_Mutex->unlock();
	}
	void Decrement() 
	{
		bool delete_flag = false;
		_Mutex->lock();
		if (-- * _UseCount == 0)
		{
			delete _Ptr;
			delete _UseCount;
			delete_flag = true;
		}
		_Mutex->unlock();
		if (delete_flag)
			delete _Mutex;
	}
private:
	T* _Ptr;			//引用计数
	size_t* _UseCount;	//指向管理资源的指针
	mutex* _Mutex;		//互斥量,保证线程安全
};

void thread_fun(my_shared_ptr<int>& sp, int n)
{
	for (int i = 0; i < n; ++i)
	{
		cout << "use_count = " << sp.use_count() << endl;
		my_shared_ptr<int> copy(sp);
	}
	cout << "use_count = " << sp.use_count() << endl;
}

int main()
{
	int n = 2;
	int* p = new int(100);

	my_shared_ptr<int> sp(p);

	thread th1(thread_fun, sp, n);
	thread th2(thread_fun, sp, n);

	//cout << "use_count = " << sp.use_count() << endl;


	th1.join();
	th2.join();

	return 0;
}

/*
int main()
{
	int* p = new int(10);
	int* q = new int(20);
	int* x = new int(30);
	my_shared_ptr<int> sp(p);

	cout << "*sp = " << *sp << endl;
	cout << "use_count = " << sp.use_count() << endl;

	my_shared_ptr<int> sp1 = sp;
	cout << "use_count = " << sp.use_count() << endl;
	
	my_shared_ptr<int> sp2(q);
	my_shared_ptr<int> sp3(sp2);
 	sp2 = sp1;
	cout << "use_count = " << sp.use_count() << endl;

	sp3.reset(x);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

/*
int main()  //shared_ptr的简单使用
{
	//每多一个对象指向p空间，引用计数就加一，对象析构则引用计数减一。引用计数为0时才允许释放p空间
	int* p = new int(10);

	//shared_ptr 共享智能指针，也叫引用计数型智能指针
	std::shared_ptr<int> sp;
	//cout << "*sp = " << *sp << endl;
	cout << "use_count = " << sp.use_count() << endl;

	//std::shared_ptr<int> sp1 = sp;  //允许拷贝构造
	//cout << "use_count = " << sp1.use_count() << endl;

	//std::shared_ptr<int> sp2;
	//sp2 = sp1;                      //允许赋值
	//cout << "use_count = " << sp2.use_count() << endl;

	sp.reset(p);  //重新设置
	cout << "*sp = " << *sp << endl;
	cout << "use_count = " << sp.use_count() << endl;

	if (sp.unique())  //判断sp是否是唯一管理p空间的智能指针
		cout << "is unique." << endl;
	else
		cout << "is not unique." << endl;

	//weak_ptr(弱指针)是为配合shared_ptr而引入的一种智能指针，它更像是shared_ptr的一个助手而不是智能指针，
	//因为它不具有普通指针的行为，没有重载operator*和->
	//它的最大作用在于协助shared_ptr工作，像旁观者那样观测资源的使用情况。
	std::weak_ptr<int> wp = sp;
	cout << "use_count = " << sp.use_count() << endl;  //引用计数不会加一
	cout << "wp_use_count = " << wp.use_count() << endl;

	return 0;
}
*/ 
