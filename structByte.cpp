#include <iostream>

using namespace std;

typedef union unRec
{
	unsigned long ul;
	unsigned short us[7];
	unsigned char uc;
}REC_S;		//16字节（按4字节对齐） 

int main()
{
	unsigned short* pucCharArray[10][10];
	REC_S st, * pst;
	printf("%d\n", sizeof(pucCharArray));	//400 
	printf("%d\n", sizeof(st));				//16
	printf("%d\n", sizeof(pst));			//4
	printf("%d\n", sizeof(*pst));			//16

	return 0;
}

/****************************************************************/

/*
struct BBB
{
	long lA1;
	char cA2;
	char cA3;
	long lA4;
	long lA5;
}*p; //16字节

int main()
{
	p = (struct BBB*)0x100000;
	printf("0x%x\n", p + 0x1);					// 0x100010
	printf("0x%x\n", (unsigned long)p + 0x1);	// 0x100001
	printf("0x%x\n", (unsigned long*)p + 0x1);	// 0x100004
	printf("0x%x\n", (char*)p + 0x1);			// 0x100001

	return 0;
}
*/
