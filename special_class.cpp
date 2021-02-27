//һ�������һ���ֻ࣬���ڶ��ϴ�������
//ʵ�ַ�ʽ��
//1. ����Ĺ��캯��˽�У���������������˽�С���ֹ���˵��ÿ�����ջ�����ɶ���
//2. �ṩһ����̬�ĳ�Ա�������ڸþ�̬��Ա��������ɶѶ���Ĵ���
class Heap_Only
{
public:
	static Heap_Only* CreateInstance_heap(int data = 0)
	{
		return new Heap_Only(data);
	}
private:
	//���캯������ջ�ϴ�������
	Heap_Only(int data = 0) : m_data(data)
	{}  
	Heap_Only(const Heap_Only& t) : m_data(t.m_data)
	{}
private:
	int m_data = 0;
};

//���������һ���ֻ࣬����ջ�ϴ�������
//����һ��ͬ�Ͻ����캯��˽�л���Ȼ����ƾ�̬�����������󷵻ؼ��ɡ�
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
//������������new
class Stack_Only_2
{
public:
	Stack_Only_2(int data = 0) : m_data(data)
	{}
private:
	//��Ϊnew�ײ�����operator new���������ε�operator new����
	void* operator new(size_t sz);

	//ע��ҲҪ��ֹ��λnew
	void* operator new(size_t sz, int pos);
private:
	int m_data = 0;
};

//���������һ���࣬���ܱ�����
//����ֻ�ᷢ�������������У��������캯���Լ���ֵ��������أ�
//�����Ҫ��һ�����ֹ������ֻ���ø��಻�ܵ��ÿ������캯���Լ���ֵ��������ؼ��ɡ�
class NoCopy : noncopyable  //boost�⣬noncopyable�Ŀ����͸�ֵ�������Ϊ��private
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
class NoCopy   //C++11������
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
class NoCopy   //C++98������
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

//�ġ������һ���࣬���ܱ��̳�

//C++11������final�ؼ��֣�final�����࣬��ʾ���಻�ܱ��̳С�
class A final
{
	// ....
};

/*
//C++98�й��캯��˽�л����������е���������Ĺ��캯�������޷��̳�
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

//�塢�����һ���ֻ࣬�ܴ���һ������(�򵥵ĵ���ģʽ)
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


