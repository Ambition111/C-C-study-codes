#include <iostream>
#include <string>
#include <vector>

using namespace std;


// 最长公共子序列
int LCSLength(char X[], char Y[], int i, int j, vector<vector<int>>& c, vector<vector<int>>& s)  //最长公共子序列的长度
{
	if (i == 0 || j == 0)	return 0;
	else if (c[i][j] > 0)	return c[i][j];
	else if (X[i] == Y[j])
	{
		c[i][j] = LCSLength(X, Y, i - 1, j - 1, c, s) + 1;
		s[i][j] = 1; 
	}
	else
	{
		int maxlen1 = LCSLength(X, Y, i - 1, j, c, s);
		int maxlen2 = LCSLength(X, Y, i, j - 1, c, s);
		if (maxlen1 > maxlen2)
		{
			c[i][j] = maxlen1;
			s[i][j] = 2;
		}
		else
		{
			c[i][j] = maxlen2;
			s[i][j] = 3;
		}
	}
	return c[i][j];
}

void Print_Vector(vector<vector<int>>& c, int m, int n)
{
	for (int i = 0; i <= m; ++i)
	{
		for (int j = 0; j <= n; ++j)
		{
			cout << c[i][j] << "  ";
		}
		cout << endl;
	}
	cout << endl;
}

void Print_String(char X[], vector<vector<int>>& s, int m, int n)
{
	if (m == 0 || n == 0)	return;
	if (s[m][n] == 1)
	{
		Print_String(X, s, m - 1, n - 1);
		cout << X[m] << " ";
	}
	else if (s[m][n] == 2)
		Print_String(X, s, m - 1, n);
	else
		Print_String(X, s, m, n - 1);
}
int main()
{
	const int m = 7;
	const int n = 6;
	char X[m + 2] = "#ABCBDAB";
	// 下标			 01234567
	char Y[n + 2] = "#BDCABA";
	// 下标			 0123456

	vector<vector<int>> c, s;
	c.resize(m + 1);
	s.resize(m + 1);
	for (int i = 0; i <= m; ++i)
	{
		c[i].resize(n + 1);
		s[i].resize(n + 1);
	}
	int maxlen = LCSLength(X, Y, m, n, c, s);	//最长公共子序列的长度

	Print_Vector(c, m, n);
	Print_Vector(s, m, n);
	cout << maxlen << endl;

	Print_String(X, s, m, n);
	return  0;
}
