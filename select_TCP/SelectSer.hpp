#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <sys/select.h>
#include "tcpser.hpp"
#include <vector>

class SelectSer
{
public:
    SelectSer()
    {
        _max_fd = -1;
        FD_ZERO(&_readfds);
    }
    ~SelectSer()
    {}

    void AddFd(int fd)
    {
        //1.添加
        FD_SET(fd, &_readfds);
        //2.更新最大文件描述符
        if (fd > _max_fd)
            _max_fd = fd;
    }
    void DeleteFd(int fd)
    {
        //1.删除
        FD_CLR(fd, &_readfds);
        //2.更新最大文件描述符
        for (int i = _max_fd; i >= 0; --i)
        {
            if (FD_ISSET(i, &_readfds) == 0) //不在
                continue;
            _max_fd = i;
            break;
        }
    }
    bool SelectWait(std::vector<TcpSer>* vec)
    {
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 3000;

        fd_set tmp = _readfds;
        int ret = select(_max_fd + 1, &tmp, NULL, NULL, &tv);
        if (ret < 0)
        {
            perror("select");
            return false;
        }
        else if (ret == 0)
        {
            printf("select timeout\n");
            return false;
        }

        //正常情况
        for (int i = 0; i <= _max_fd; ++i)
        {
            if (FD_ISSET(i, &tmp))
            {
                //返回就绪的文件描述符 i
                //返回TCP类的对象
                TcpSer ts;
                ts.SetFd(i);
                vec->push_back(ts);
            }
        }
        return true;
    }
private:
    int _max_fd;    //保存最大文件描述符
    fd_set _readfds;//可读事件集合
};
