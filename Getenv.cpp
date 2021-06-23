#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 

#if 0 
// 第一种方法：利用 main 函数的参数获取环境变量 
int main(int argc, char* argv[], char* env[])
{
    //argc --> 命令行参数个数
    //argv --> 命令行参具体的内容
    //env  --> 环境变量
    
    //char* env[]
    //env[0] = xxx
    //env[1] = yyy
    //env[2] = xxx
    //env[n] = NULL		//最后一个环境变量用NULL标识
    for(int i = 0; env[i]; i++)
    {
        printf("%s\n", env[i]);
    }
    return 0;
}
#endif


#if 0
// 第二种方法：利用 libc 库当中提供的变量来获取
int main()
{
    //libc 库
    extern char** environ;
    for(int i = 0; environ[i]; i++)
    {
        printf("%s\n", environ[i]);
    }
    return 0;
}
#endif


// 第三种方法：利用 libc 库提供的函数getenv()来获取一个具体的环境变量的值。要引入 stdlib.h 头文件
int main()
{
    //libc 库当中提供的 函数老获取某一个具体的环境变量的值
    //库函数
    //char *getenv(const char *name);
    //    name : 环境变量的名称
    //    返回值：具体环境变量所对应的值
    char* PATH = getenv("PATH");
    printf("PATH=%s\n", PATH);
    return 0;
}


