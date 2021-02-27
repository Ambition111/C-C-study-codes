//一、请设计一个类，只能在堆上创建对象
//实现方式：
//1. 将类的构造函数私有，拷贝构造声明成私有。防止别人调用拷贝在栈上生成对象。
//2. 提供一个静态的成员函数，在该静态成员函数中完成堆对象的创建
class Heap_Only
{
public:
	static Heap_Only* CreateInstance_heap(int data = 0)
	{
		return new Heap_Only(data);
	}
private:
	//构造函数会在栈上创建对象
	Heap_Only(int data = 0) : m_data(data)
	{}  
	Heap_Only(const Heap_Only& t) : m_data(t.m_data)
	{}
private:
	int m_data = 0;
};

//二、请设计一个类，只能在栈上创建对象
//方法一：同上将构造函数私有化，然后设计静态方法创建对象返回即可。
class Stack_Only_1
{
public:
	static Stack_Only_1 CreateInstance_stack(int data = 0)
	{
		return Stack_Only_1(data);
	}
private:
	Stack_Only_1(int data = 0) : m_data(data)
	{}
private: 
	int m_data = 0;
};
//方法二：屏蔽new
class Stack_Only_2
{
public:
	Stack_Only_2(int data = 0) : m_data(data)
	{}
private:
	//因为new底层会调用operator new，所以屏蔽掉operator new即可
	void* operator new(size_t sz);

	//注意也要防止定位new
	void* operator new(size_t sz, int pos);
private:
	int m_data = 0;
};

//三、请设计一个类，不能被拷贝
//拷贝只会发生在两个场景中：拷贝构造函数以及赋值运算符重载，
//因此想要让一个类禁止拷贝，只需让该类不能调用拷贝构造函数以及赋值运算符重载即可。
class NoCopy : noncopyable  //boost库，noncopyable的拷贝和赋值语句声明为了private
{
public:
	NoCopy(int data = 0) : m_data(data)
	{}
	NoCopy(const NoCopy& t);
	NoCopy& operator=(const NoCopy& t);
private:
	int m_data = 0;
};
/*
class NoCopy   //C++11的做法
{
public:
	NoCopy(int data = 0) : m_data(data)
	{}
	NoCopy(const NoCopy& t) = delete;
	NoCopy& operator=(const NoCopy& t) = delete;
private:
	int m_data = 0;
};
*/

/*	
class NoCopy   //C++98的做法
{
public:
	NoCopy(int data = 0) : m_data(data)
	{}
private:
	NoCopy(const NoCopy& t) : m_data(t.m_data)
	{}
	NoCopy& operator=(const NoCopy& t);
private:
	int m_data = 0;
};
*/

//四、请设计一个类，不能被继承

//C++11方法：final关键字，final修饰类，表示该类不能被继承。
class A final
{
	// ....
};

/*
//C++98中构造函数私有化，派生类中调不到基类的构造函数。则无法继承
class NonInherit
{
public:
	static NonInherit GetInstance()
	{
		return NonInherit();
	}
private:
	NonInherit()
	{}

private:
	int m_data = 0;
};
*/

//五、请设计一个类，只能创建一个对象(简单的单例模式)
class Test
{
public:
	static Test* CreateInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new Test;
		}
		return _instance;
	}
protected:
	Test()
	{}
private:
	static Test* _instance;
};
Test* Test::_instance = nullptr;


