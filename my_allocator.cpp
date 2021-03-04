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
			exit(1);  //异常退出
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
		//在现有p的空间里面，构造一个初始值为value的T1对象
		new (p) T1(value);		//定位new   placement new
	}
	template<class T>
	void _destroy(T* ptr)
	{
		ptr->~T();
	}

	//空间配置器
	template<class T>
	class allocator
	{
	public:
		typedef T value_type;
		typedef T* pointer;      //指针类型
		typedef T& reference;	 //引用类型
		typedef size_t size_type;//大小类型
	public:
		//申请空间
		pointer allocate(size_type n, const void* hint = 0)
		{
			return _allocate(n, (pointer)0);
		}
		//释放空间
		void deallocate(pointer p, size_type n)
		{
			_deallocate(p);
		}
		//构造对象
		void construct(pointer p, const value_type& value)
		{
			_construct(p, value);
		}
		//析构对象
		void destroy(pointer p)
		{
			_destroy(p);
		}
	};
}




int main()
{
	my_allocator::allocator<int> alloc;
	int* ptr = alloc.allocate(10);  // 申请空间

	for (int i = 0; i < 10; ++i)
	{
		ptr[i] = i;
	}
	for (int i = 0; i < 10; ++i)
	{
		cout << ptr[i] << " ";
	}
	cout << endl;

	alloc.deallocate(ptr, 10);  // 释放空间

	return 0;
}

