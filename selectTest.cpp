#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>


/*
    简单使用select
          1.准备需要监控的文件描述符
          2.将文件描述符添加到对应的事件集合中
          3.监控
          4.判断文件描述符是否在集合中
          5.处理文件描述符
*/

int main()
{
begin:
    //监控0号文件描述符的可读事件
    fd_set readfds;         //定义集合
    FD_ZERO(&readfds);      //将位图中各位置为0
    FD_SET(0, &readfds);    //由于要监控0号文件描述符，就将第0位置1

    //增加超时时间
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;

    //int ret = select(0 + 1, &readfds, NULL, NULL, NULL);  //阻塞监控
    int ret = select(0 + 1, &readfds, NULL, NULL, &tv);  //带有超时时间的监控

    if (ret < 0)    //监控出错
    {
        perror("select");
        return 0;
    }
    else if (ret == 0) //等待超时
    {
        printf("select timeout\n");
        //TODO
        //循环监控，返回时只返回就绪的文件描述符
        ret = FD_ISSET(0, &readfds); //超时后判断0号描述符到底在不在集合中
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

    //大于0，正常返回就绪件描述符的个数
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

