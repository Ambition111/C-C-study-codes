#pragma once

#include <iostream>

using namespace std;


//一级空间配置器
template<int inst>
class __malloc_alloc_template
{
public:
	static void* allocate(size_t n)  //malloc
	{
		void* result = malloc(n);
		if (result == nullptr)
		{
			//内存不足时的补救方法
			result = oom_malloc(n);  // out of memory 内存不足时的申请空间函数
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
	//函数名：set_malloc_handler
	//返回值：指向set_malloc_handler(void(*f)())的函数指针
	static void(*set_malloc_handler(void(*f)()))()  //仿照C++11中的set_new_handler机制
	{
		//把所设置的函数设置到静态的函数指针成员里
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

			(*my_malloc_handler)(); //处理方法
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

			(*my_malloc_handler)(); //处理方法
			//my_malloc_handler();
			result = realloc(p, new_sz);
			if (result)
				return result;
		}
	}
private:
	static void(*__malloc_alloc_oom_handler)(); //函数指针，指向的函数没有返回值和参数
};
