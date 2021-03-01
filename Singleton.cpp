//����ģʽ����������������֮ǰ���Ѿ�ʵ������һ������
// �ŵ㣺��
// ȱ�㣺���ܻᵼ�½�����������������ж�����������ʵ������˳��ȷ����

class Singleton_hungry
{
public:
	static Singleton_hungry* GetInstance()
	{
		return &m_instance;
	}
private:
	//���캯��˽��
	Singleton_hungry() {};
	//C++11 ������
	Singleton_hungry(const Singleton_hungry&) = delete;
	Singleton_hungry& operator=(const Singleton_hungry&) = delete;
private:
	static Singleton_hungry m_instance;
};

Singleton_hungry Singleton_hungry::m_instance;

/////////////////////////////////////////////////////////////////////////
//����ģʽ��ʲôʱ����Ҫʹ�ö���ʲôʱ���ٴ�������
//	�������������ʮ�ֺ�ʱ����ռ�úܶ���Դ��������ز������ ��ʼ���������Ӱ�����ȡ�ļ�����
//	�ȣ����п��ܸö����������ʱ�����õ�����ôҲҪ�ڳ���һ��ʼ�ͽ��г�ʼ�����ͻᵼ�³�������ʱ
//	�ǳ��Ļ����� �����������ʹ������ģʽ���ӳټ��أ����á�
class Singleton_lazy
{
public:
	static Singleton_lazy* CreateInstance()
	{
		// Double-Check ˫�����,��֤Ч�ʺ��̰߳�ȫ
		//��һ���̶߳���Ϊ�ж�Ϊ�ռ����󣬺������д����߳�ͬʱ���У��жϲ�Ϊ��ʱֱ��return�����������
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

