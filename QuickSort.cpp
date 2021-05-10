// 双向划分
int Partition(vector<int>& arr, int left, int right)
{
	//if (arr.empty())	return -1;	// 非递归时不用写判空
	int i = left, j = right;
	int tmp = arr[left];
	
	while (i < j)
	{
		while (i < j && arr[j] >= tmp)	--j;
		arr[i] = arr[j];
		while (i < j && arr[i] < tmp)	++i;
		arr[j] = arr[i];
	}
	arr[i] = tmp;
	return i;
}

// 单向划分
int oneWayPartition(vector<int>& arr, int left, int right)
{
	//if (arr.empty())	return -1;
	int i = left, j = left + 1;
	int tmp = arr[i];
	while (j <= right)
	{
		if (arr[j] <= tmp)
		{
			++i;
			std::swap(arr[i], arr[j]);
		}		
		++j;
	}
	std::swap(arr[left], arr[i]);
	return i;
}
// 随机数划分
int RandPartition(vector<int>& arr, int left, int right)
{
	int index = rand() % (right - left + 1) + left;
	std::swap(arr[left], arr[index]);
	return Partition(arr, left, right);
}

void Quick(vector<int>& arr, int left, int right)
{
	if (left < right)
	{
		int mid = RandPartition(arr, left, right);
		Quick(arr, left, mid - 1);
		Quick(arr, mid + 1, right);
	}
}

// 递归
void QuickSort1(vector<int>& arr)
{
	Quick(arr, 0, arr.size() - 1);
}

// 非递归
void QuickSort2(vector<int>& arr)
{
	if (arr.empty())	return;
	int left = 0, right = arr.size() - 1;

	queue<int> qu;
	qu.push(left);
	qu.push(right);
	while (!qu.empty())
	{
		left = qu.front();
		qu.pop();
		right = qu.front();
		qu.pop();
		int mid = oneWayPartition(arr, left, right);
		if (left < mid - 1)
		{
			qu.push(left);
			qu.push(mid - 1);
		}
		if (mid + 1 < right)
		{
			qu.push(mid + 1);
			qu.push(right);
		}
	}
}
