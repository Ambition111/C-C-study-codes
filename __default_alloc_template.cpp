//申请空间够大，超过128 bytes时，就由一级空间配置器处理；小于128 bytes时，则以内存池管理，即二级空间配置器

//二级空间配置器

enum { __ALIGN = 8 };
enum { __MAX_BYTES = 128 };
enum { __NFREELISTS = __MAX_BYTES / __ALIGN };  //128/8=16 //自由链表

template<bool thread, int inst>
class __default_alloc_template
{
public:
	static void* allocate(size_t n);
	static void deallocate(void* p, size_t n);
	static void* reallocate(void* p, size_t old_sz, size_t new_sz);
private:
	union obj
	{
		union obj* free_list_link;  //链表指针
		char client_data[1];		//用户数据
	};
private:
	static void* refill(size_t n);//填充空间的函数
	static char* chunk_alloc(size_t size, int& nobjs);
private:
	static size_t FREELIST_INDEX(size_t bytes)
	{	//计算申请字节的大小应该在FREELIST的哪个下标下申请
		return (bytes + __ALIGN - 1) / __ALIGN - 1;
	}
	static size_t ROUND_UP(size_t bytes)
	{	//将传进的bytes提升为8的倍数
		return (bytes + __ALIGN - 1) & ~(__ALIGN - 1);
	}
	
private:
	static obj* free_list[__NFREELISTS];
private:
	static char* start_free;	//起始位置
	static char* end_free;		//结束位置
	static size_t heap_size;	//堆区大小
};
//程序进入主函数之前系统会为静态成员分配空间
template<bool thread, int inst>
typename __default_alloc_template<thread, inst>::obj*
__default_alloc_template<thread, inst>::free_list[__NFREELISTS] =
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

template<bool thread, int inst>
char* __default_alloc_template<thread, inst>::start_free = 0;
template<bool thread, int inst>
char* __default_alloc_template<thread, inst>::end_free = 0;
template<bool thread, int inst>
size_t __default_alloc_template<thread, inst>::heap_size = 0;

template<bool thread, int inst>
void* __default_alloc_template<thread, inst>::allocate(size_t n)
{
	//如果申请空间大小超过128 bytes，就交给一级空间配置器申请
	if (n > __MAX_BYTES)
		return malloc_alloc::allocate(n);

	obj** my_free_list;
	obj* result;

	my_free_list = free_list + FREELIST_INDEX(n);
	result = *my_free_list;
	if (result == 0) //没有空间
	{
		void* r = refill(ROUND_UP(n)); //填充空间
		return r;
	}
	*my_free_list = result->free_list_link;  // p = q->next
	return result;
}
template<bool thread, int inst>
void __default_alloc_template<thread, inst>::deallocate(void* p, size_t n)
{
	obj* q = (obj*)p;
	obj** my_free_list;

	if (n > __MAX_BYTES)	//一级空间配置器申请
		malloc_alloc::deallocate(p, n);  //一级空间配置器释放

	my_free_list = free_list + FREELIST_INDEX(n);
	q->free_list_link = *my_free_list;
	*my_free_list = q;
}
template<bool thread, int inst>
void* __default_alloc_template<thread, inst>::reallocate(void* p, size_t old_sz, size_t new_sz);
template<bool thread, int inst>
void* __default_alloc_template<thread, inst>::refill(size_t n)
{
	void* result;
	obj* cur_obj, *next_obj; //当前obj和下一个obj的指针

	int nobjs = 20; //申请20个块数。前人经验表明：10个太小需要经常申请浪费效率，30个太大浪费空间
	char* chunk = chunk_alloc(n, nobjs);

	obj** my_free_list;
	my_free_list = free_list + FREELIST_INDEX(n);
	result = (obj*)chunk;

	*my_free_list = next_obj = (obj*)(chunk + n);
	for (int i = 1; ; ++i)
	{
		cur_obj = next_obj;
		next_obj = (obj*)((char*)next_obj + n);
		if (nobjs - 1 == i)
		{
			cur_obj->free_list_link = 0;
			break;
		}
		else
			cur_obj->free_list_link = next_obj;
	}
	return result;
}
template<bool thread, int inst>
char* __default_alloc_template<thread, inst>::chunk_alloc(size_t size, int& nobjs)
{
	size_t total_bytes = size * nobjs; //要申请的字节大小
	size_t bytes_left = end_free - start_free; //内存池剩余空间

	char* result = 0;
	if (bytes_left >= total_bytes)
	{
		//剩余空间足够分配
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else if (bytes_left >= size)
	{
		//剩余空间至少满足一个块的申请
		nobjs = bytes_left / size; //更新块数
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else
	{
		//剩余空间不足
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
		
		//有剩余的内存池空间，但不满足于本次的申请
		if (bytes_left > 0)
		{
			obj** my_free_list = free_list + FREELIST_INDEX(bytes_left);
			((obj*)start_free)->free_list_link = *my_free_list;
			*my_free_list = (obj*)start_free;
		}
		start_free = (char*)malloc(bytes_to_get);
		if (start_free == 0)
		{	//申请空间失败
			obj** my_free_list;
			obj* p;
			for (size_t i = size; i <= __MAX_BYTES; i += __ALIGN)
			{
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (p != 0)
				{
					*my_free_list = p->free_list_link;
					start_free = (char*)p;
					end_free = start_free + i;
					return chunk_alloc(size, nobjs);
				}
			}
			end_free = nullptr;
			start_free = (char*)malloc_alloc::allocate(bytes_to_get);
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return chunk_alloc(size, nobjs);
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////////////////

//空间配置器的再次封装
//在C++中，用户所需空间可能是任意类型的，有单个对象空间，有连续空间，每次让用户自己计算所需空间
//总大小不是很友好，因此SGI - STL将空间配置器重新再封装了一层

#ifdef __USE_MALLOC
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;
#else
typedef __default_alloc_template<0, 0> alloc;
#endif

// 注意：该类只负责申请与归还对象的空间，不负责空间中对象的构造与析构
template<class T, class Alloc>
class simple_alloc
{
public:
	//动态开辟数组空间
	static T* allocate(size_t n)
	{
		return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
	}
	//动态开辟单个对象空间
	static T* allocate()
	{
		return (T*)Alloc::allocate(sizeof(T));
	}
	//动态释放数组空间
	static void deallocate(T* p, size_t n)
	{
		if (0 != n)
			Alloc::deallocate(p, n * sizeof(T));
	}
	//动态释放单个空间
	static void deallocate(T* p)
	{
		Alloc::deallocate(p, sizeof(T));
	}
};
/////////////////////////////////////////////////////////////////////////////////////
/*
	内存池：先申请一块比较大的内存块已做备用，当需要内存时，直接到内存池中去去，当池中空间不够时，再向内存中去取；
	当用户不用时，直接还回内存池即可。避免了频繁向系统申请小块内存所造成的效率低、内存碎片以及额外浪费的问题
问题：
	1、当用户需要空间时，能否直接从内存池中大块空间空间中直接截取？为什么？
	答：不能，会造成内存碎片，因为内存池实现时分割的内存块是连续的，如果直接截取，会很容易造成碎片。
		 应该优先从链表中选，先从大块中拿，否则如果用户需要大块的空间，可能就给不了了
	2、对用户归还的空间能否直接拼接在大块内存前？
	答：不能，应该回收到当前的自由链表里面去。
	3、对用户归还的空间如何进行管理？
	答：采用哈希桶的方式进行管理。
	4、不断切割会有什么后果？
	答：碎片
*/



