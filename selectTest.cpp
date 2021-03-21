#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>


/*
    ��ʹ��select
          1.׼����Ҫ��ص��ļ�������
          2.���ļ���������ӵ���Ӧ���¼�������
          3.���
          4.�ж��ļ��������Ƿ��ڼ�����
          5.�����ļ�������
*/

int main()
{
begin:
    //���0���ļ��������Ŀɶ��¼�
    fd_set readfds;         //���弯��
    FD_ZERO(&readfds);      //��λͼ�и�λ��Ϊ0
    FD_SET(0, &readfds);    //����Ҫ���0���ļ����������ͽ���0λ��1

    //���ӳ�ʱʱ��
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;

    //int ret = select(0 + 1, &readfds, NULL, NULL, NULL);  //�������
    int ret = select(0 + 1, &readfds, NULL, NULL, &tv);  //���г�ʱʱ��ļ��

    if (ret < 0)    //��س���
    {
        perror("select");
        return 0;
    }
    else if (ret == 0) //�ȴ���ʱ
    {
        printf("select timeout\n");
        //TODO
        //ѭ����أ�����ʱֻ���ؾ������ļ�������
        ret = FD_ISSET(0, &readfds); //��ʱ���ж�0�������������ڲ��ڼ�����
        if (ret == 0)
        {
            printf("0 fd is not in readfds\n");
            goto begin;
        }
        else if (ret != 0)
        {
            printf("0 fd is in readfds\n");
            return 0;
        }
    }

    //����0���������ؾ������������ĸ���
    printf("ret : %d\n", ret);

    if (FD_ISSET(0, &readfds) != 0)
    {
        char buf[1024] = {0};
        read(0, buf, sizeof(buf) - 1);
        printf("buf = %s\n", buf);
    }

    FD_CLR(0, &readfds);
    return 0;
}

