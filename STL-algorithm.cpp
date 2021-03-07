#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <forward_list>
#include <list>
#include <set>
//#include <functional>

using namespace std;



//next_permutation��prev_permutation(��Ҫ)
//	next_permutation�ǻ�ȡһ���������һ�����У����Ա���ȫ���У�
//	prev_permutation�պ��෴����ȡһ�����е�ǰһ������
	//������ {a,b,c}��ÿһ��Ԫ�ض��Ⱥ����С�������ֵ����У��̶�a֮��a��bc��С��c��b��������һ�����м�Ϊ{a,c,b}��
	//��{a,c,b}����һ�����м�Ϊ{a,b,c}��ͬ������Ƴ����е���������Ϊ��
	//{a,b,c}��{a,c,b}��{b,a,c}��{b,c,a}��{c,a,b}��{c,b,a}��
	//����{a,b,c}û����һ��Ԫ�أ�{c,b,a}û����һ��Ԫ��
int main()
{
	vector<int> v = { 2,1,3 };

	sort(v.begin(), v.end());	//1 2 3,����Ҫ�ú�����
	do
	{
		cout << v[0] << " " << v[1] << " " << v[2] << endl;
	} while (next_permutation(v.begin(), v.end()));

	cout << "-----------------------------" << endl;

	sort(v.begin(), v.end(), greater<int>());	//3 2 1,����Ҫ��ǰ����
	do
	{
		cout << v[0] << " " << v[1] << " " << v[2] << endl;
	} while (prev_permutation(v.begin(), v.end()));

	return 0;
}

/*
// unique() �ú�����������ɾ�����������ڵ��ظ���Ԫ�أ�ȷ��Ԫ��Ψһ�ԣ�
//          ע����ʹ��ǰҪ��֤�����е�Ԫ��������ģ����ܴﵽ������ȥ�ء�
// ע�⣺
	//1. �ú��������ǽ��ظ��Ե�Ԫ��ɾ�����������ú��治�ظ���Ԫ�ؽ�ǰ���ظ���Ԫ�ظ��ǵ��ˡ�
	//2. ����ֵ��һ����������ָ�����ȥ�غ������в��ظ����һ��Ԫ�ص���һ��λ�á�
	//3. �ú�����Ҫ���erase()���������Ľ�Ԫ��ɾ����
int main()
{
	vector<int> v1 = { 4,1,5,8,5,0,5,5,5,9,5,3,7,2,6,5 };

	cout << "ɾ��ǰ��";
	for (const auto& e : v1)
		cout << e << " ";
	cout << endl;

	cout << "ɾ����";
	sort(v1.begin(), v1.end());
	auto it = unique(v1.begin(), v1.end());
	v1.erase(it, v1.end());
	for (const auto& e : v1)
		cout << e << " ";
	cout << endl;
	return 0;
}



/*
// reverse() ���㷨�������Ƕ������е�Ԫ�ؽ�������
int main()
{
	vector<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };		//����OK
	//list<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };		//����OK
	//set<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };		//����error����Ϊreverse()��ı�map/set�Ĺؼ�ֵ����map/set������ؼ�ֵ���޸�

	cout << "����ǰ��";
	for (const auto& e : v1)
		cout << e << " ";
	cout << endl;

	cout << "���ú�";
	reverse(v1.begin(), v1.end());
	for (const auto& e : v1)
		cout << e << " ";
	cout << endl;

	return 0;
}


/*
bool isOdd(int x) //x�Ƿ�Ϊ����
{
	return (x % 2 == 1);
}

// partition() ���㷨�������ǰ��������������е�Ԫ�ؽ��л���;ֻ���𻮷ֲ�����
int main()
{
	vector<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };

	//�����Ƿ�Ϊ��������Ϊ������
	auto pos = partition(v1.begin(), v1.end(), isOdd);

	cout << "�������֣�";
	for (auto it = v1.begin(); it != pos; ++it)
		cout << *it << " ";
	cout << endl;
	cout << "ż�����֣�";
	for (auto it = pos; it != v1.end(); ++it)
		cout << *it << " ";
	cout << endl;
	return 0;
}


/*
//partial_sort() ���������㷨 ���㷨�������ǣ���TOPK
//partial_sort��ʵ��ԭ���ǣ�
//		��ԭʼ����������Ϊ[first, middle)��Ԫ��ִ��make_heap()��������һ�����ѣ�
//		Ȼ����[middle, last)�е�ÿ��Ԫ�غ�first���бȽϣ�first�ڵ�Ԫ��Ϊ���ڵ����ֵ��
//		���С�ڸ����ֵ���򻥻�Ԫ��λ�ã�����[first, middle)�ڵ�Ԫ�ؽ��е�����ʹ�䱣��������
//		�Ƚ���֮���ڶ�[first, middle)�ڵ�Ԫ����һ�ζ�����sort_heap()������ʹ�䰴�������С�
//		ע�⣬����������ǲ�ͬ��(����ֻ�Ǳ�֤ÿ�����ڵ�������Һ���)����˸��㷨�Ĺ���ʵ�ʾ��ǣ�TOP - K

int main()
{
	// �Ҹ�������ǰ5����С��Ԫ��, Ԫ�����մ洢��v1��ǰ5��λ�ã���ֻ��ǰ5��Ԫ�ؽ�������
	vector<int> v1{ 4, 1, 8, 0, 5, 9, 3, 7, 2, 6 };

	//partial_sort(v1.begin(), v1.begin() + 5, v1.end()); //Ĭ��С����
	partial_sort(v1.begin(), v1.begin() + 5, v1.end(), greater<int>()); //�º����������

	cout << "TOP-5: ";
	for (int i = 0; i < 5; ++i)
		cout << v1.at(i) << " "; //v1.[i]
	cout << endl;

	return 0;
}

/*
// merge() ���㷨���ý������������кϲ���һ����������
int main()
{
	//vector<int> v1 = { 1,3,5,7,9 };
	//vector<int> v2 = { 2,4,6,8,10 };

	//Ҳ���Խ���ͬ����������ֵ�ϲ�
	forward_list<int> v1 = { 1,3,5,7,9 };
	vector<int> v2 = { 2,4,6,8,10 };

	//���Ժϲ���һ��������
	//vector<int> res(v1.size() + v2.size());
	//merge(v1.begin(), v1.end(), v2.begin(), v2.end(), res.begin());
	
	//Ҳ���Ժϲ���һ��������
	int arr[10] = { 0 };
	//ʹ��ǰ��Ҫ��֤������������//O(m+n)
	merge(v1.begin(), v1.end(), v2.begin(), v2.end(), arr);

	for (const auto& e : arr)
		cout << e << " ";
	cout << endl;

	return 0;
}

/*
struct Greater3  //x�Ƿ����3
{
	bool operator()(int x)
	{
		return x > 3;
	}
};

//find()��find_if() ���㷨����������Ԫ���������е�һ�γ��ֵ�λ��
int main()
{
	vector<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };

	auto rit = find(v1.begin(), v1.end(), 11);	//����ֵΪ����������
	if (rit != v1.end())
		cout << "rit = " << *rit << endl;
	else
		cout << "not find" << endl;

	//���ҵ�һ������3������
	auto rit1 = find_if(v1.begin(), v1.end(), Greater3());
	if (rit1 != v1.end())
		cout << "rit1 = " << *rit1 << endl;
	else
		cout << "not find" << endl;
	return 0;
}

/*
bool isOdd(int x) //x�Ƿ�Ϊ����
{
	return (x % 2 == 1);
}
struct Greater3  //x�Ƿ����3
{
	bool operator()(int x)
	{
		return x > 3;
	}
};

//count()��count_if() ���㷨��������ͳ��������ĳ��Ԫ�س��ֵĴ�����
int main()
{
	vector<int> v = { 10,20,10,20,10,10,30 };
	//��ֵͳ�� ��count()
	cout << "count of 10: " << count(v.begin(), v.end(), 10) << endl;
	
	vector<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };
	//��������ͳ�ƣ�count_if()
	cout << "count of odd: " << count_if(v1.begin(), v1.end(), isOdd) << endl; //���Դ��뺯����ַ
	cout << "count of Greater3: " << count_if(v1.begin(), v1.end(), Greater3()) << endl; //Ҳ���Դ��뺯�����󣨷º�������ʵ������С���ţ�д����������



	return 0;
}
/*
// accumulate() ���㷨�����Ƕ������е�Ԫ�ؽ����ۼӡ�
int main()
{
	vector<int> v = { 1,2,3,4,5,6,7,8,9,10 };

	//��ͨ�ۼӷ�
	int sum = 0;
	for (const auto& e : v)
	{
		sum += e;
	}
	cout << "sum = " << sum << endl;

	//STL�ۼ��㷨
	int sum1 = accumulate(v.begin(), v.end(), 0); //sum1 = 55 //0��ʾ��0��ʼ��
	cout << "sum1 = " << sum1 << endl;

	auto pos = find(v.begin(), v.end(), 6);
	int sum2 = accumulate(v.begin(), pos, 0);  //sum1 = 15;��ʾ�ӿ�ʼ�ӵ�posλ�õ�ǰһ����

	cout << "sum2 = " << sum2 << endl;

	return 0;
}*/

