#include <iostream>
#include <vector>
//#include <vld.h>

using namespace std;

namespace my_allocator
{
	template<class T>
	T* _allocate(size_t size, T*)
	{
		T* tmp = (T*)::operator new(size * sizeof(T));
		if (tmp == nullptr)
		{
			cout << "out of memory." << endl;
			exit(1);  //�쳣�˳�
		}
		return tmp;
	}
	template<class T>
	void _deallocate(T* buffer)
	{
		::operator delete(buffer);
	}
	template<class T1, class T2>
	void _construct(T1* p, const T2& value)
	{
		//������p�Ŀռ����棬����һ����ʼֵΪvalue��T1����
		new (p) T1(value);		//��λnew   placement new
	}
	template<class T>
	void _destroy(T* ptr)
	{
		ptr->~T();
	}

	//�ռ�������
	template<class T>
	class allocator
	{
	public:
		typedef T value_type;
		typedef T* pointer;      //ָ������
		typedef T& reference;	 //��������
		typedef size_t size_type;//��С����
	public:
		//����ռ�
		pointer allocate(size_type n, const void* hint = 0)
		{
			return _allocate(n, (pointer)0);
		}
		//�ͷſռ�
		void deallocate(pointer p, size_type n)
		{
			_deallocate(p);
		}
		//�������
		void construct(pointer p, const value_type& value)
		{
			_construct(p, value);
		}
		//��������
		void destroy(pointer p)
		{
			_destroy(p);
		}
	};
}




int main()
{
	my_allocator::allocator<int> alloc;
	int* ptr = alloc.allocate(10);  // ����ռ�

	for (int i = 0; i < 10; ++i)
	{
		ptr[i] = i;
	}
	for (int i = 0; i < 10; ++i)
	{
		cout << ptr[i] << " ";
	}
	cout << endl;

	alloc.deallocate(ptr, 10);  // �ͷſռ�

	return 0;
}

