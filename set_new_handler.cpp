#include<iostream>

using namespace std;


void OutOfMemory()
{
	cout<<"Out Of Memory."<<endl; //delete 
	exit(1);
}

void main()
{
	set_new_handler(OutOfMemory); //用于new开辟空间时预防内存不足的情况
	int *ptr = nullptr;
	try
	{
		ptr = new int[536870911]; //空间不足
	}
	catch(bad_alloc &e)
	{
		cout<<e.what()<<endl;
		return;
	}

	cout<<"new OK"<<endl;
	delete []ptr;
}
