//����ռ乻�󣬳���128 bytesʱ������һ���ռ�����������С��128 bytesʱ�������ڴ�ع����������ռ�������

//�����ռ�������

enum { __ALIGN = 8 };
enum { __MAX_BYTES = 128 };
enum { __NFREELISTS = __MAX_BYTES / __ALIGN };  //128/8=16 //��������

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
		union obj* free_list_link;  //����ָ��
		char client_data[1];		//�û�����
	};
private:
	static void* refill(size_t n);//���ռ�ĺ���
	static char* chunk_alloc(size_t size, int& nobjs);
private:
	static size_t FREELIST_INDEX(size_t bytes)
	{	//���������ֽڵĴ�СӦ����FREELIST���ĸ��±�������
		return (bytes + __ALIGN - 1) / __ALIGN - 1;
	}
	static size_t ROUND_UP(size_t bytes)
	{	//��������bytes����Ϊ8�ı���
		return (bytes + __ALIGN - 1) & ~(__ALIGN - 1);
	}
	
private:
	static obj* free_list[__NFREELISTS];
private:
	static char* start_free;	//��ʼλ��
	static char* end_free;		//����λ��
	static size_t heap_size;	//������С
};
//�������������֮ǰϵͳ��Ϊ��̬��Ա����ռ�
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
	//�������ռ��С����128 bytes���ͽ���һ���ռ�����������
	if (n > __MAX_BYTES)
		return malloc_alloc::allocate(n);

	obj** my_free_list;
	obj* result;

	my_free_list = free_list + FREELIST_INDEX(n);
	result = *my_free_list;
	if (result == 0) //û�пռ�
	{
		void* r = refill(ROUND_UP(n)); //���ռ�
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

	if (n > __MAX_BYTES)	//һ���ռ�����������
		malloc_alloc::deallocate(p, n);  //һ���ռ��������ͷ�

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
	obj* cur_obj, *next_obj; //��ǰobj����һ��obj��ָ��

	int nobjs = 20; //����20��������ǰ�˾��������10��̫С��Ҫ���������˷�Ч�ʣ�30��̫���˷ѿռ�
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
	size_t total_bytes = size * nobjs; //Ҫ������ֽڴ�С
	size_t bytes_left = end_free - start_free; //�ڴ��ʣ��ռ�

	char* result = 0;
	if (bytes_left >= total_bytes)
	{
		//ʣ��ռ��㹻����
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else if (bytes_left >= size)
	{
		//ʣ��ռ���������һ���������
		nobjs = bytes_left / size; //���¿���
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else
	{
		//ʣ��ռ䲻��
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
		
		//��ʣ����ڴ�ؿռ䣬���������ڱ��ε�����
		if (bytes_left > 0)
		{
			obj** my_free_list = free_list + FREELIST_INDEX(bytes_left);
			((obj*)start_free)->free_list_link = *my_free_list;
			*my_free_list = (obj*)start_free;
		}
		start_free = (char*)malloc(bytes_to_get);
		if (start_free == 0)
		{	//����ռ�ʧ��
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

//�ռ����������ٴη�װ
//��C++�У��û�����ռ�������������͵ģ��е�������ռ䣬�������ռ䣬ÿ�����û��Լ���������ռ�
//�ܴ�С���Ǻ��Ѻã����SGI - STL���ռ������������ٷ�װ��һ��

#ifdef __USE_MALLOC
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;
#else
typedef __default_alloc_template<0, 0> alloc;
#endif

// ע�⣺����ֻ����������黹����Ŀռ䣬������ռ��ж���Ĺ���������
template<class T, class Alloc>
class simple_alloc
{
public:
	//��̬��������ռ�
	static T* allocate(size_t n)
	{
		return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
	}
	//��̬���ٵ�������ռ�
	static T* allocate()
	{
		return (T*)Alloc::allocate(sizeof(T));
	}
	//��̬�ͷ�����ռ�
	static void deallocate(T* p, size_t n)
	{
		if (0 != n)
			Alloc::deallocate(p, n * sizeof(T));
	}
	//��̬�ͷŵ����ռ�
	static void deallocate(T* p)
	{
		Alloc::deallocate(p, sizeof(T));
	}
};
/////////////////////////////////////////////////////////////////////////////////////
/*
	�ڴ�أ�������һ��Ƚϴ���ڴ���������ã�����Ҫ�ڴ�ʱ��ֱ�ӵ��ڴ����ȥȥ�������пռ䲻��ʱ�������ڴ���ȥȡ��
	���û�����ʱ��ֱ�ӻ����ڴ�ؼ��ɡ�������Ƶ����ϵͳ����С���ڴ�����ɵ�Ч�ʵ͡��ڴ���Ƭ�Լ������˷ѵ�����
���⣺
	1�����û���Ҫ�ռ�ʱ���ܷ�ֱ�Ӵ��ڴ���д��ռ�ռ���ֱ�ӽ�ȡ��Ϊʲô��
	�𣺲��ܣ�������ڴ���Ƭ����Ϊ�ڴ��ʵ��ʱ�ָ���ڴ���������ģ����ֱ�ӽ�ȡ��������������Ƭ��
		 Ӧ�����ȴ�������ѡ���ȴӴ�����ã���������û���Ҫ���Ŀռ䣬���ܾ͸�������
	2�����û��黹�Ŀռ��ܷ�ֱ��ƴ���ڴ���ڴ�ǰ��
	�𣺲��ܣ�Ӧ�û��յ���ǰ��������������ȥ��
	3�����û��黹�Ŀռ���ν��й���
	�𣺲��ù�ϣͰ�ķ�ʽ���й���
	4�������и����ʲô�����
	����Ƭ
*/



