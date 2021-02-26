#include <iostream>
#include <memory>
#include <vld.h>
#include <vector>
#include <boost/smart_ptr.hpp>

using namespace boost;
using namespace std;

int main()
{
	//boost库里的scoped_ptr与C++11里的unique_ptr是一样的
	//scoped_ptr也具有auto_ptr同样的缺陷，不能用作容器的元素，但原因不同
	//auto_ptr是因为它的转移语义，而scoped_ptr则是因为不支持拷贝赋值，不符合容器对元素类型的要求

	int* p = new int(10);

	//局部智能指针，不允许拥有权发生转移
	scoped_ptr<int> sp(p); //这个智能指针只能在本作用域里使用，不希望被转让
	cout << "*sp = " << *sp << endl;

	scoped_ptr<string> sp1(new string("helloBoost"));
	cout << sp1->size() << endl;
	
	//scoped_ptr不允许拷贝赋值
	//scoped_ptr<int> sp2 = sp;  //error
	//sp2 = sp;                  //error

	return 0;
}


////////////////////////////////////////////////////////////////////////////////
/*
//注意：auto_ptr只能释放单个空间，不能将数组空间交给智能指针

//拥有权的转移问题 --> 这是智能指针的核心问题
template<class Type>
class my_auto_ptr
{
public:
	my_auto_ptr(Type* _P = 0) : _Owns(_P != 0), _Ptr(_P)
	{}
	
	my_auto_ptr(const my_auto_ptr<Type>& p) : _Owns(p._Owns), _Ptr(p.release())
	{}
	my_auto_ptr<Type>& operator=(const my_auto_ptr<Type>& p)
	{
		if (this != &p)
		{
			if (_Ptr != p._Ptr) //如果赋值前_Ptr还指向别的空间
			{
				if (_Owns)       //如果拥有权为真
					delete _Ptr; //释放_Ptr指向的空间
				_Owns = p._Owns;
			}
			else if (p._Owns)  //如果两者指向相同空间，并且p.Owns为真
				_Owns = true; 

			_Ptr = p.release();  //让p失去拥有权后给_Ptr赋值
		}
		return *this;
	}
	~my_auto_ptr()
	{
		if (_Owns)   //如果拥有权为真，就将_Ptr所指的空间释放
			delete _Ptr;
	}
public:
	Type& operator*() const
	{
		return *getPtr();
	}
	Type* operator->() const
	{
		return getPtr();
	}
	Type* getPtr() const
	{
		return _Ptr;
	}
public:
	//const my_auto_ptr<Type> *const this
	Type* release() const //释放拥有权
	{
		((my_auto_ptr<Type>* const)this)->_Owns = false; //为了保留拷贝构造传入对象的常性,去掉this的常性 //方法一
		//_Owns = false; // 方法二，将_Owns声明为mutable;

		Type* _tmp = _Ptr;
		((my_auto_ptr<Type>* const)this)->_Ptr = nullptr; //彻底转移

		return _tmp;
	}
private:
	//mutable 表示变量是可变的
	mutable bool _Owns;  //拥有权
	Type* _Ptr;	 //管理的指针
};

class Test
{
public:
	void Fun()
	{
		cout << "This is Test::Fun()" << endl;
	}
};

int main()
{
	int* p = new int(10);
	my_auto_ptr<int> ap(p);   //RAII

	//使用默认的拷贝构造函数(浅拷贝)是简单的赋值操作，即把ap1也指向ap所指的空间;
	//而在析构时就会尝试释放同一块空间造成程序崩溃。默认的赋值语句也是如此
	my_auto_ptr<int> ap1 = ap; //拥有权也将转移，实则就是将ap的拥有权(_Owns)置为false，但是在vc(C++98)里的auto_ptr底层里，*ap仍然能访问p里的数据
	
	cout << "*ap1 = " << *ap1 << endl;
	//cout << "*ap = " << *ap << endl;    //vc6.0 下仍可访问。//我们不想让其访问

	//int* q = new int(20);
	//my_auto_ptr<int> ap2(q);  //情况一

	my_auto_ptr<int> ap2;       //情况二
	ap2 = ap1;  //赋值


	return 0;
}

/*
int main()
{
	int* p = new int(10);
	my_auto_ptr<int> ap(p);

	//重载ap的* ->
	cout << *ap << endl;

	my_auto_ptr<Test> ap1(new Test);
	ap1->Fun();


	return 0;
}
/////////////////////////////////////////////////////////////////////
/*
int main()
{
	int* p = new int(10);

	auto_ptr<int> ap(p);

	auto_ptr<int> ap(p);   //智能指针（自动指针）
	cout << *ap << endl;
	//delete p;


	return 0;
}
*/
