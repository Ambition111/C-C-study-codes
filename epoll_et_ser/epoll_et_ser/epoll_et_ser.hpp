#pragma once

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include "tcpser.hpp"
#include <vector>

class EpollSer
{
public:
	EpollSer()
	{
		_epoll_fd = epoll_create(10);
		if (_epoll_fd < 0)
		{
			exit(1);
		}
	}
	~EpollSer()
	{
	}
public:
	//对于ET模式，需要判断一下是否要开启ET模式
	bool AddFd(int fd, bool is_et = false)
	{
		struct epoll_event ev;
		if (is_et)
			ev.events = EPOLLIN | EPOLLET;
		else
			ev.events = EPOLLIN;
		ev.data.fd = fd;

		int ret = epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &ev);
		if (ret < 0)
		{
			perror("epoll_ctl");
			return false;
		}
		return true;
	}
	bool DeleteFd(int fd)
	{
		int ret = epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
		if (ret < 0)
		{
			perror("epoll_ctl");
			return false;
		}
		return true;
	}
	bool EpollWait(std::vector<TcpSer>* vec)	//vec出参
	{
		struct epoll_event fd_arr[10];	//事件结构数组

		//ret：返回就绪的文件描述符个数
		int ret = epoll_wait(_epoll_fd, fd_arr, sizeof(fd_arr) / sizeof(fd_arr[0]), -1);
		if (ret < 0)
		{
			perror("epoll_wait");
			return false;
		}
		else if (ret == 0)
		{
			printf("epoll_wait timeout\n");
			return false;
		}

		//防止数组越界
		if (ret > (int)sizeof(fd_arr)/sizeof(fd_arr[0]))
		{
			ret = sizeof(fd_arr)/sizeof(fd_arr[0]);
		}
		//ret > 0
		for (int i = 0; i < ret; ++i)
		{
			TcpSer ts;
			ts.SetFd(fd_arr[i].data.fd);
			vec->push_back(ts);
		}
		return true;
	}
private:
	int _epoll_fd;		//epoll操作句柄
};

