#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 

#if 0 
// ��һ�ַ��������� main �����Ĳ�����ȡ�������� 
int main(int argc, char* argv[], char* env[])
{
    //argc --> �����в�������
    //argv --> �����вξ��������
    //env  --> ��������
    
    //char* env[]
    //env[0] = xxx
    //env[1] = yyy
    //env[2] = xxx
    //env[n] = NULL		//���һ������������NULL��ʶ
    for(int i = 0; env[i]; i++)
    {
        printf("%s\n", env[i]);
    }
    return 0;
}
#endif


#if 0
// �ڶ��ַ��������� libc �⵱���ṩ�ı�������ȡ
int main()
{
    //libc ��
    extern char** environ;
    for(int i = 0; environ[i]; i++)
    {
        printf("%s\n", environ[i]);
    }
    return 0;
}
#endif


// �����ַ��������� libc ���ṩ�ĺ���getenv()����ȡһ������Ļ���������ֵ��Ҫ���� stdlib.h ͷ�ļ�
int main()
{
    //libc �⵱���ṩ�� �����ϻ�ȡĳһ������Ļ���������ֵ
    //�⺯��
    //char *getenv(const char *name);
    //    name : ��������������
    //    ����ֵ�����廷����������Ӧ��ֵ
    char* PATH = getenv("PATH");
    printf("PATH=%s\n", PATH);
    return 0;
}


