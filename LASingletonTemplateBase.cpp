//线程不安全
template<typename T>
class LASingletonTemplateBase    //单件模式的模板基类
{
private:
	static T* sm_instance;
protected:
	LASingletonTemplateBase()
	{
		assert(sm_instance == 0);
		sm_instance = static_cast<T*>(this);
	}
	virtual ~LASingletonTemplateBase()
	{
		assert(sm_instance != 0);
		sm_instance = 0;
	}
public:
	static T* get_instance_ptr() //获取对象的指针
	{
		if (sm_instance == 0)
		{
			sm_instance = new T();
		}
		return sm_instance;
	}
	static T& get_instance_ref() //获取对象的引用
	{
		if (sm_instance == 0)
		{
			sm_instance = new T();
		}
		return *sm_instance;
	}
	static void remove_instance() //删除一个对象
	{
		assert(sm_instance);
		if (sm_instance)
		{
			delete sm_instance;
		}
		assert(sm_instance == 0);
	}
};
template<typename T>
T* LASingletonTemplateBase<T>::sm_instance = 0;

class Test1
{
public:
	Test1()
	{
		cout << "Test1::Test1()" << endl;
	}
	~Test1()
	{
		cout << "Test1::~Test1()" << endl;
	}
private:
	int m_data = 0;
};
int main()
{
	//单件模式的模板基类构造对象，四个指针是同一个地址，即四个对象是同一个对象
	Test1* pt1 = LASingletonTemplateBase<Test1>::get_instance_ptr();
	Test1* pt2 = LASingletonTemplateBase<Test1>::get_instance_ptr();
	Test1* pt3 = LASingletonTemplateBase<Test1>::get_instance_ptr();
	Test1* pt4 = LASingletonTemplateBase<Test1>::get_instance_ptr();
	
	//正常构造对象，与上面4个对象是不同的
	Test1*pt = new Test1();

	return 0;
}
