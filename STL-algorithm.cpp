#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <forward_list>
#include <list>
#include <set>
//#include <functional>

using namespace std;



//next_permutation和prev_permutation(重要)
//	next_permutation是获取一个排序的下一个排列，可以遍历全排列，
//	prev_permutation刚好相反，获取一个排列的前一个排列
	//对序列 {a,b,c}，每一个元素都比后面的小，按照字典序列，固定a之后，a比bc都小，c比b大，它的下一个序列即为{a,c,b}，
	//而{a,c,b}的上一个序列即为{a,b,c}，同理可以推出所有的六个序列为：
	//{a,b,c}、{a,c,b}、{b,a,c}、{b,c,a}、{c,a,b}、{c,b,a}，
	//其中{a,b,c}没有上一个元素，{c,b,a}没有下一个元素
int main()
{
	vector<int> v = { 2,1,3 };

	sort(v.begin(), v.end());	//1 2 3,所以要用后排列
	do
	{
		cout << v[0] << " " << v[1] << " " << v[2] << endl;
	} while (next_permutation(v.begin(), v.end()));

	cout << "-----------------------------" << endl;

	sort(v.begin(), v.end(), greater<int>());	//3 2 1,所以要用前排列
	do
	{
		cout << v[0] << " " << v[1] << " " << v[2] << endl;
	} while (prev_permutation(v.begin(), v.end()));

	return 0;
}

/*
// unique() 该函数的作用是删除区间中相邻的重复性元素，确保元素唯一性，
//          注意在使用前要保证区间中的元素是有序的，才能达到真正的去重。
// 注意：
	//1. 该函数并不是将重复性的元素删除掉，而是用后面不重复的元素将前面重复的元素覆盖掉了。
	//2. 返回值是一个迭代器，指向的是去重后容器中不重复最后一个元素的下一个位置。
	//3. 该函数需要配合erase()才能真正的将元素删除掉
int main()
{
	vector<int> v1 = { 4,1,5,8,5,0,5,5,5,9,5,3,7,2,6,5 };

	cout << "删除前：";
	for (const auto& e : v1)
		cout << e << " ";
	cout << endl;

	cout << "删除后：";
	sort(v1.begin(), v1.end());
	auto it = unique(v1.begin(), v1.end());
	v1.erase(it, v1.end());
	for (const auto& e : v1)
		cout << e << " ";
	cout << endl;
	return 0;
}



/*
// reverse() 该算法的作用是对区间中的元素进行逆置
int main()
{
	vector<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };		//逆置OK
	//list<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };		//逆置OK
	//set<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };		//逆置error，因为reverse()会改变map/set的关键值，而map/set不允许关键值被修改

	cout << "逆置前：";
	for (const auto& e : v1)
		cout << e << " ";
	cout << endl;

	cout << "逆置后：";
	reverse(v1.begin(), v1.end());
	for (const auto& e : v1)
		cout << e << " ";
	cout << endl;

	return 0;
}


/*
bool isOdd(int x) //x是否为奇数
{
	return (x % 2 == 1);
}

// partition() 该算法的作用是按照条件对区间中的元素进行划分;只负责划分不排序
int main()
{
	vector<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };

	//按照是否为奇数划分为两部分
	auto pos = partition(v1.begin(), v1.end(), isOdd);

	cout << "奇数部分：";
	for (auto it = v1.begin(); it != pos; ++it)
		cout << *it << " ";
	cout << endl;
	cout << "偶数部分：";
	for (auto it = pos; it != v1.end(); ++it)
		cout << *it << " ";
	cout << endl;
	return 0;
}


/*
//partial_sort() 部分排序算法 该算法的作用是：找TOPK
//partial_sort的实现原理是：
//		对原始容器内区间为[first, middle)的元素执行make_heap()操作构造一个最大堆，
//		然后拿[middle, last)中的每个元素和first进行比较，first内的元素为堆内的最大值。
//		如果小于该最大值，则互换元素位置，并对[first, middle)内的元素进行调整，使其保持最大堆序。
//		比较完之后在对[first, middle)内的元素做一次对排序sort_heap()操作，使其按增序排列。
//		注意，堆序和增序是不同的(堆序只是保证每个根节点大于左右孩子)。因此该算法的功能实际就是：TOP - K

int main()
{
	// 找该区间中前5个最小的元素, 元素最终存储在v1的前5个位置，即只对前5哥元素进行排序
	vector<int> v1{ 4, 1, 8, 0, 5, 9, 3, 7, 2, 6 };

	//partial_sort(v1.begin(), v1.begin() + 5, v1.end()); //默认小根堆
	partial_sort(v1.begin(), v1.begin() + 5, v1.end(), greater<int>()); //仿函数：大根堆

	cout << "TOP-5: ";
	for (int i = 0; i < 5; ++i)
		cout << v1.at(i) << " "; //v1.[i]
	cout << endl;

	return 0;
}

/*
// merge() 该算法作用将两个有序序列合并成一个有序序列
int main()
{
	//vector<int> v1 = { 1,3,5,7,9 };
	//vector<int> v2 = { 2,4,6,8,10 };

	//也可以将不同两个容器的值合并
	forward_list<int> v1 = { 1,3,5,7,9 };
	vector<int> v2 = { 2,4,6,8,10 };

	//可以合并到一个向量里
	//vector<int> res(v1.size() + v2.size());
	//merge(v1.begin(), v1.end(), v2.begin(), v2.end(), res.begin());
	
	//也可以合并到一个数组里
	int arr[10] = { 0 };
	//使用前提要保证两个序列有序、//O(m+n)
	merge(v1.begin(), v1.end(), v2.begin(), v2.end(), arr);

	for (const auto& e : arr)
		cout << e << " ";
	cout << endl;

	return 0;
}

/*
struct Greater3  //x是否大于3
{
	bool operator()(int x)
	{
		return x > 3;
	}
};

//find()、find_if() 该算法的作用是找元素在区间中第一次出现的位置
int main()
{
	vector<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };

	auto rit = find(v1.begin(), v1.end(), 11);	//返回值为迭代器类型
	if (rit != v1.end())
		cout << "rit = " << *rit << endl;
	else
		cout << "not find" << endl;

	//查找第一个大于3的数据
	auto rit1 = find_if(v1.begin(), v1.end(), Greater3());
	if (rit1 != v1.end())
		cout << "rit1 = " << *rit1 << endl;
	else
		cout << "not find" << endl;
	return 0;
}

/*
bool isOdd(int x) //x是否为奇数
{
	return (x % 2 == 1);
}
struct Greater3  //x是否大于3
{
	bool operator()(int x)
	{
		return x > 3;
	}
};

//count()与count_if() 该算法的作用是统计区间中某个元素出现的次数。
int main()
{
	vector<int> v = { 10,20,10,20,10,10,30 };
	//按值统计 ：count()
	cout << "count of 10: " << count(v.begin(), v.end(), 10) << endl;
	
	vector<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };
	//根据条件统计：count_if()
	cout << "count of odd: " << count_if(v1.begin(), v1.end(), isOdd) << endl; //可以传入函数地址
	cout << "count of Greater3: " << count_if(v1.begin(), v1.end(), Greater3()) << endl; //也可以传入函数对象（仿函数，其实重载了小括号，写出来像函数）



	return 0;
}
/*
// accumulate() 该算法作用是对区间中的元素进行累加。
int main()
{
	vector<int> v = { 1,2,3,4,5,6,7,8,9,10 };

	//普通累加法
	int sum = 0;
	for (const auto& e : v)
	{
		sum += e;
	}
	cout << "sum = " << sum << endl;

	//STL累加算法
	int sum1 = accumulate(v.begin(), v.end(), 0); //sum1 = 55 //0表示从0开始加
	cout << "sum1 = " << sum1 << endl;

	auto pos = find(v.begin(), v.end(), 6);
	int sum2 = accumulate(v.begin(), pos, 0);  //sum1 = 15;表示从开始加到pos位置的前一个数

	cout << "sum2 = " << sum2 << endl;

	return 0;
}*/

