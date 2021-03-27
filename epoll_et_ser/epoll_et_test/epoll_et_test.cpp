/*====================================================
*   Copyright (C) 2021  All rights reserved.
*
*   文件名称：epoll_et_test.cpp
*   创 建 者：phw
*   创建日期：2021年03月22日
*   描    述：
================================================================*/
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string>
#include <fcntl.h>
#include <errno.h>

using namespace std;
int main(int argc, char *argv[])
{
	//设置文件描述符属性为非阻塞,为了解决ET模式下为了一次将数据读完出现的饥饿问题
	int flag = fcntl(0, F_GETFL);
	fcntl(0, F_SETFL, flag | O_NONBLOCK);

	//1.创建epoll操作句柄
	int epoll_fd = epoll_create(10);
	if (epoll_fd < 0)
	{
		perror("epoll_create");
		return 0;
	}

	//2.添加文件描述符
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;	//或上EPOLLET，表示边缘触发方式
	ev.data.fd = 0;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &ev);

	//3.监听
	while (1)
	{
		struct epoll_event fd_arr[10];
		int ret = epoll_wait(epoll_fd, fd_arr, sizeof(fd_arr)/sizeof(fd_arr[0]), -1);
		if (ret < 0)
		{
			perror("epoll_wait");
			//return 0;
			continue;
		}

		for (int i = 0; i < ret; ++i)
		{
			if (fd_arr[i].data.fd == 0)
			{
				std::string read_data;
				while (1)
				{
					char buf[3] = {0};	//除去'\0'一次只能读2个字节

					//文件描述符为非阻塞属性时，如果read读到空，就返回-1
					ssize_t readsize = read(0, buf, sizeof(buf) - 1);
					if (readsize < 0)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
						{
							//说明数据读完了
							goto myend;
						}
						perror("read");
						return 0;
					}
					read_data += buf;

					//ET+循环:会造成饥饿状态，比如输入nihaoa再按回车的时候，read每次读2个字节，最后读到'\n',由于'\n'占一个字节，此时readsize<2，程序正常输出。但是当输入nihao再按回车时，最后read读到的是"o\n"，两个字节，此时readsize不小于2，程序继续循环read，但是此时输入的"nihao\n"已经读完了，程序不能正常输出，造成饥饿状态。
					if (readsize < (ssize_t)sizeof(buf) - 1)
					{
					myend:
						printf("read buf content is %s\n", read_data.c_str());
						break;
					}
				}
			}
		}
	}
	//4.处理就绪事件
    return 0;
}

