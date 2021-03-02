#include<iostream>

using namespace std;


void OutOfMemory()
{
	cout<<"Out Of Memory."<<endl; //delete 
	exit(1);
}

void main()
{
	set_new_handler(OutOfMemory); //����new���ٿռ�ʱԤ���ڴ治������
	int *ptr = nullptr;
	try
	{
		ptr = new int[536870911]; //�ռ䲻��
	}
	catch(bad_alloc &e)
	{
		cout<<e.what()<<endl;
		return;
	}

	cout<<"new OK"<<endl;
	delete []ptr;
}
