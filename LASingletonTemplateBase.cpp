//�̲߳���ȫ
template<typename T>
class LASingletonTemplateBase    //����ģʽ��ģ�����
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
	static T* get_instance_ptr() //��ȡ�����ָ��
	{
		if (sm_instance == 0)
		{
			sm_instance = new T();
		}
		return sm_instance;
	}
	static T& get_instance_ref() //��ȡ���������
	{
		if (sm_instance == 0)
		{
			sm_instance = new T();
		}
		return *sm_instance;
	}
	static void remove_instance() //ɾ��һ������
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
	//����ģʽ��ģ����๹������ĸ�ָ����ͬһ����ַ�����ĸ�������ͬһ������
	Test1* pt1 = LASingletonTemplateBase<Test1>::get_instance_ptr();
	Test1* pt2 = LASingletonTemplateBase<Test1>::get_instance_ptr();
	Test1* pt3 = LASingletonTemplateBase<Test1>::get_instance_ptr();
	Test1* pt4 = LASingletonTemplateBase<Test1>::get_instance_ptr();
	
	//�����������������4�������ǲ�ͬ��
	Test1*pt = new Test1();

	return 0;
}
