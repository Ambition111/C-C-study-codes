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
	std::weak_ptr<listNode> prev;//ʹ��weak_ptr����Ϊ����������prev��next�����ü������������ѭ������
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

//����ɾ����
void Del(listNode* pn)
{
	cout << "Delete pn" << endl;
	delete[] pn;
}
int main()
{
	listNode* pa = new listNode[5];
	
	//boost::shared_array<listNode> spa(pa);  //OK

	//C++11����û����ν��shared_array,��boost�����У�������������std::shared_ptr��������ռ䣬����Ҫ����ɾ��������ʵ����ɾ������
	//std::shared_ptr<listNode> spa(pa);	  //error
	std::shared_ptr<listNode> spa(pa,Del);	  //OK

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/*
//ģ��ʵ��shared_ptr
template<class T>
class my_shared_ptr
{
public:
	my_shared_ptr(T* _P = 0) : _Ptr(_P), _UseCount((size_t*)new int(1)), _Mutex(new mutex)
	{}
	my_shared_ptr(const my_shared_ptr<T>& p) : _Ptr(p._Ptr), _UseCount(p._UseCount), _Mutex(p._Mutex)
	{
		Increment();//�������ü���
	}
	my_shared_ptr<T>& operator=(const my_shared_ptr<T>& p)
	{
		if (this != &p)
		{
			//�ͷ���Դ
			Decrement();
			_Ptr = p._Ptr;
			_UseCount = p._UseCount;
			Increment();
		}
		return *this;
	}
	~my_shared_ptr()
	{
		Decrement();//�������ü���
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
		++*_UseCount;		//�ٽ���
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
	T* _Ptr;			//���ü���
	size_t* _UseCount;	//ָ�������Դ��ָ��
	mutex* _Mutex;		//������,��֤�̰߳�ȫ
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
int main()  //shared_ptr�ļ�ʹ��
{
	//ÿ��һ������ָ��p�ռ䣬���ü����ͼ�һ���������������ü�����һ�����ü���Ϊ0ʱ�������ͷ�p�ռ�
	int* p = new int(10);

	//shared_ptr ��������ָ�룬Ҳ�����ü���������ָ��
	std::shared_ptr<int> sp;
	//cout << "*sp = " << *sp << endl;
	cout << "use_count = " << sp.use_count() << endl;

	//std::shared_ptr<int> sp1 = sp;  //����������
	//cout << "use_count = " << sp1.use_count() << endl;

	//std::shared_ptr<int> sp2;
	//sp2 = sp1;                      //����ֵ
	//cout << "use_count = " << sp2.use_count() << endl;

	sp.reset(p);  //��������
	cout << "*sp = " << *sp << endl;
	cout << "use_count = " << sp.use_count() << endl;

	if (sp.unique())  //�ж�sp�Ƿ���Ψһ����p�ռ������ָ��
		cout << "is unique." << endl;
	else
		cout << "is not unique." << endl;

	//weak_ptr(��ָ��)��Ϊ���shared_ptr�������һ������ָ�룬��������shared_ptr��һ�����ֶ���������ָ�룬
	//��Ϊ����������ָͨ�����Ϊ��û������operator*��->
	//���������������Э��shared_ptr���������Թ��������۲���Դ��ʹ�������
	std::weak_ptr<int> wp = sp;
	cout << "use_count = " << sp.use_count() << endl;  //���ü��������һ
	cout << "wp_use_count = " << wp.use_count() << endl;

	return 0;
}
*/ 
