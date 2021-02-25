#include <iostream>

using namespace std;

class Base
{
public:
	virtual void fun_1()
	{
		cout << "This is Base::fun_1()" << endl;
	}
	void fun_2()
	{
		cout << "This is Base::fun_2()" << endl;
	}
	virtual Base* fun_3()
	{
		cout << "This is Base::fun_3()" << endl;
		return new Base;
	}
	virtual ~Base()
	{
		cout << "This is Base::~Base()" << endl;
	}
};

class D : public Base
{
public:
	void fun_1()   //派生类重写基类虚方法时可以不用加 virtual 关键字
	{
		cout << "This is D::fun_1()" << endl;
	}
	void fun_2(int a)  //派生类函数与基类函数造成同名隐藏(重定义)
	{
		cout << "This is D::fun_2()" << endl;
	}
	virtual D* fun_3()  //基类与派生类虚函数返回值类型不同(协变)，
	{
		cout << "This is D::fun_3()" << endl;
		return new D;
	}
	virtual ~D()  //析构函数的重写(基类与派生类析构函数的名字不同)
	{
		cout << "This is D::~D()" << endl;
	}
};

int main()
{
	Base* p1 = new(D);

	//p1->fun_1();  //实现多态，基类指针调用派生类方法
	//p1->fun_2();
	p1->fun_3();


	//// 只有派生类Student的析构函数重写了Person的析构函数，下面的delete对象调用析构函数，
	//才能构成多态，才能保证p1和p2指向的对象正确的调用析构函数。
	//Base* p1 = new Base;
	//D* p2 = new D;


	//delete p1;
	//delete p2;
	return 0;
}
