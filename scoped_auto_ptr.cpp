#include <iostream>
#include <memory>
#include <vld.h>
#include <vector>
#include <boost/smart_ptr.hpp>

using namespace boost;
using namespace std;

int main()
{
	//boost�����scoped_ptr��C++11���unique_ptr��һ����
	//scoped_ptrҲ����auto_ptrͬ����ȱ�ݣ���������������Ԫ�أ���ԭ��ͬ
	//auto_ptr����Ϊ����ת�����壬��scoped_ptr������Ϊ��֧�ֿ�����ֵ��������������Ԫ�����͵�Ҫ��

	int* p = new int(10);

	//�ֲ�����ָ�룬������ӵ��Ȩ����ת��
	scoped_ptr<int> sp(p); //�������ָ��ֻ���ڱ���������ʹ�ã���ϣ����ת��
	cout << "*sp = " << *sp << endl;

	scoped_ptr<string> sp1(new string("helloBoost"));
	cout << sp1->size() << endl;
	
	//scoped_ptr����������ֵ
	//scoped_ptr<int> sp2 = sp;  //error
	//sp2 = sp;                  //error

	return 0;
}


////////////////////////////////////////////////////////////////////////////////
/*
//ע�⣺auto_ptrֻ���ͷŵ����ռ䣬���ܽ�����ռ佻������ָ��

//ӵ��Ȩ��ת������ --> ��������ָ��ĺ�������
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
			if (_Ptr != p._Ptr) //�����ֵǰ_Ptr��ָ���Ŀռ�
			{
				if (_Owns)       //���ӵ��ȨΪ��
					delete _Ptr; //�ͷ�_Ptrָ��Ŀռ�
				_Owns = p._Owns;
			}
			else if (p._Owns)  //�������ָ����ͬ�ռ䣬����p.OwnsΪ��
				_Owns = true; 

			_Ptr = p.release();  //��pʧȥӵ��Ȩ���_Ptr��ֵ
		}
		return *this;
	}
	~my_auto_ptr()
	{
		if (_Owns)   //���ӵ��ȨΪ�棬�ͽ�_Ptr��ָ�Ŀռ��ͷ�
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
	Type* release() const //�ͷ�ӵ��Ȩ
	{
		((my_auto_ptr<Type>* const)this)->_Owns = false; //Ϊ�˱����������촫�����ĳ���,ȥ��this�ĳ��� //����һ
		//_Owns = false; // ����������_Owns����Ϊmutable;

		Type* _tmp = _Ptr;
		((my_auto_ptr<Type>* const)this)->_Ptr = nullptr; //����ת��

		return _tmp;
	}
private:
	//mutable ��ʾ�����ǿɱ��
	mutable bool _Owns;  //ӵ��Ȩ
	Type* _Ptr;	 //�����ָ��
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

	//ʹ��Ĭ�ϵĿ������캯��(ǳ����)�Ǽ򵥵ĸ�ֵ����������ap1Ҳָ��ap��ָ�Ŀռ�;
	//��������ʱ�ͻ᳢���ͷ�ͬһ��ռ���ɳ��������Ĭ�ϵĸ�ֵ���Ҳ�����
	my_auto_ptr<int> ap1 = ap; //ӵ��ȨҲ��ת�ƣ�ʵ����ǽ�ap��ӵ��Ȩ(_Owns)��Ϊfalse��������vc(C++98)���auto_ptr�ײ��*ap��Ȼ�ܷ���p�������
	
	cout << "*ap1 = " << *ap1 << endl;
	//cout << "*ap = " << *ap << endl;    //vc6.0 ���Կɷ��ʡ�//���ǲ����������

	//int* q = new int(20);
	//my_auto_ptr<int> ap2(q);  //���һ

	my_auto_ptr<int> ap2;       //�����
	ap2 = ap1;  //��ֵ


	return 0;
}

/*
int main()
{
	int* p = new int(10);
	my_auto_ptr<int> ap(p);

	//����ap��* ->
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

	auto_ptr<int> ap(p);   //����ָ�루�Զ�ָ�룩
	cout << *ap << endl;
	//delete p;


	return 0;
}
*/
