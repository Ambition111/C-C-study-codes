#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;


void call_back()
{
    printf("even_thread��������odd_thread���ڽ���\n");
    sleep(3);
    pthread_mutex_unlock(&mutex);
}

void* odd_thread()  //��ӡ�������߳�
{
    int i;
    for (i = 1; ; i += 2)
    {
        pthread_cleanup_push(call_back, NULL);
        pthread_mutex_lock(&mutex);

        printf("odd() : \n", i);
        //usleep(100000);

        pthread_mutex_unlock(&mutex);
        pthread_cleanup_pop(1); //�������������Ϊ0ʱ�ͻ���ûص�����call_back()��������

    }
}

void* even_thread() //��ӡż�����߳�
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

    pthread_t odd_id;   //�����߳�
    pthread_t even_id;  //ż���߳�

    pthread_create(&odd_id, NULL, odd_thread, NULL);
    pthread_create(&even_id, NULL, even_thread, NULL);

    //ȡ����ӡ�������߳�,�����ڲ�֪��odd_thread���Ա�ȡ����ʱ���������odd_thread����ʱȡ��,����û�н�����even_thread�ͻᱻ������
    pthread_cancel(odd_id); //ִ��cancel����ûص�����

    pthread_join(odd_id, NULL);
    pthread_join(even_id, NULL);

    pthread_mutex_destroy(&mutex);
}

