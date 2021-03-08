#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;


void call_back()
{
    printf("even_thread被阻塞，odd_thread正在解锁\n");
    sleep(3);
    pthread_mutex_unlock(&mutex);
}

void* odd_thread()  //打印奇数的线程
{
    int i;
    for (i = 1; ; i += 2)
    {
        pthread_cleanup_push(call_back, NULL);
        pthread_mutex_lock(&mutex);

        printf("odd() : \n", i);
        //usleep(100000);

        pthread_mutex_unlock(&mutex);
        pthread_cleanup_pop(1); //这个函数参数不为0时就会调用回调函数call_back()？？？？

    }
}

void* even_thread() //打印偶数的线程
{
    int i;
    for (i = 0; ; i += 2)
    {
        pthread_mutex_lock(&mutex);

        printf("even() : %d\n", i);
        //usleep(100000);

        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    pthread_mutex_init(&mutex, NULL);

    pthread_t odd_id;   //奇数线程
    pthread_t even_id;  //偶数线程

    pthread_create(&odd_id, NULL, odd_thread, NULL);
    pthread_create(&even_id, NULL, even_thread, NULL);

    //取消打印奇数的线程,但由于不知道odd_thread可以被取消的时机，如果在odd_thread加锁时取消,由于没有解锁，even_thread就会被阻塞掉
    pthread_cancel(odd_id); //执行cancel会调用回调函数

    pthread_join(odd_id, NULL);
    pthread_join(even_id, NULL);

    pthread_mutex_destroy(&mutex);
}

