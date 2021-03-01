//饿汉模式：程序运行主函数之前就已经实例化了一个对象
// 优点：简单
// 缺点：可能会导致进程启动慢，且如果有多个单例类对象实例启动顺序不确定。

class Singleton_hungry
{
public:
	static Singleton_hungry* GetInstance()
	{
		return &m_instance;
	}
private:
	//构造函数私有
	Singleton_hungry() {};
	//C++11 防拷贝
	Singleton_hungry(const Singleton_hungry&) = delete;
	Singleton_hungry& operator=(const Singleton_hungry&) = delete;
private:
	static Singleton_hungry m_instance;
};

Singleton_hungry Singleton_hungry::m_instance;

/////////////////////////////////////////////////////////////////////////
//懒汉模式：什么时候需要使用对象，什么时候再创建对象。
//	如果单例对象构造十分耗时或者占用很多资源，比如加载插件啊， 初始化网络连接啊，读取文件啊等
//	等，而有可能该对象程序运行时不会用到，那么也要在程序一开始就进行初始化，就会导致程序启动时
//	非常的缓慢。 所以这种情况使用懒汉模式（延迟加载）更好。
class Singleton_lazy
{
public:
	static Singleton_lazy* CreateInstance()
	{
		// Double-Check 双检机制,保证效率和线程安全
		//第一个线程对象为判断为空加锁后，后面若有大量线程同时运行，判断不为空时直接return不会大量阻塞
		if (_instance == nullptr)	
		{
			mt.lock();
			if (_instance == nullptr)
			{
				_instance = new Singleton_lazy;
			}
			mt.unlock();
		}
		return _instance;
	}
protected:
	Singleton_lazy()
	{}
private:
	static Singleton_lazy* _instance;
};
Singleton_lazy* Singleton_lazy::_instance = nullptr;

