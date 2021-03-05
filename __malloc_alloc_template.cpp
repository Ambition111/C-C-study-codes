#pragma once

#include <iostream>

using namespace std;


//һ���ռ�������
template<int inst>
class __malloc_alloc_template
{
public:
	static void* allocate(size_t n)  //malloc
	{
		void* result = malloc(n);
		if (result == nullptr)
		{
			//�ڴ治��ʱ�Ĳ��ȷ���
			result = oom_malloc(n);  // out of memory �ڴ治��ʱ������ռ亯��
		}
		return result;
	}
	static void deallocate(void* p, size_t n) //free
	{
		free(p);
	}
	static void* reallocate(void* p, size_t old_sz, size_t new_sz) //realloc
	{
		void* result = realloc(p, new_sz);
		if (result == nullptr)
		{
			result = oom_realloc(p, new_sz);
		}
		return result;
	}
	//��������set_malloc_handler
	//����ֵ��ָ��set_malloc_handler(void(*f)())�ĺ���ָ��
	static void(*set_malloc_handler(void(*f)()))()  //����C++11�е�set_new_handler����
	{
		//�������õĺ������õ���̬�ĺ���ָ���Ա��
		void(*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return old;
	}
private:
	static void* oom_malloc(size_t n)
	{
		void(*my_malloc_handler)();
		void* result;

		for (; ;)
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (my_malloc_handler == 0)
				throw bad_alloc();

			(*my_malloc_handler)(); //������
			//my_malloc_handler();
			result = malloc(n);
			if (result)
				return result;
		}
	}
	static void* oom_realloc(void* p, size_t new_sz)
	{
		void(*my_malloc_handler)();
		void* result;

		for (; ;)
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (my_malloc_handler == 0)
				throw bad_alloc();

			(*my_malloc_handler)(); //������
			//my_malloc_handler();
			result = realloc(p, new_sz);
			if (result)
				return result;
		}
	}
private:
	static void(*__malloc_alloc_oom_handler)(); //����ָ�룬ָ��ĺ���û�з���ֵ�Ͳ���
};
