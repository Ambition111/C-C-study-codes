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
	void fun_1()   //��������д�����鷽��ʱ���Բ��ü� virtual �ؼ���
	{
		cout << "This is D::fun_1()" << endl;
	}
	void fun_2(int a)  //�����ຯ������ຯ�����ͬ������(�ض���)
	{
		cout << "This is D::fun_2()" << endl;
	}
	virtual D* fun_3()  //�������������麯������ֵ���Ͳ�ͬ(Э��)��
	{
		cout << "This is D::fun_3()" << endl;
		return new D;
	}
	virtual ~D()  //������������д(�������������������������ֲ�ͬ)
	{
		cout << "This is D::~D()" << endl;
	}
};

int main()
{
	Base* p1 = new(D);

	//p1->fun_1();  //ʵ�ֶ�̬������ָ����������෽��
	//p1->fun_2();
	p1->fun_3();


	//// ֻ��������Student������������д��Person�����������������delete�����������������
	//���ܹ��ɶ�̬�����ܱ�֤p1��p2ָ��Ķ�����ȷ�ĵ�������������
	//Base* p1 = new Base;
	//D* p2 = new D;


	//delete p1;
	//delete p2;
	return 0;
}
