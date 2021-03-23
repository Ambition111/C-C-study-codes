#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <stdio.h>


using namespace std;
int main(int argc, char *argv[])
{
	//����epoll������� 
	int epollfd = epoll_create(10);
	if (epollfd < 0)
	{
		perror("epoll_create");
		return 0;
	}

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = 0;
	//����ļ���������epoll_event�¼��ṹ���� 
	epoll_ctl(epollfd, EPOLL_CTL_ADD, 0, &ev);

	while (1)
	{
		struct epoll_event fd_arr[10];
		//��� 
		int ret = epoll_wait(epollfd, fd_arr, sizeof(fd_arr) / sizeof(fd_arr[0]), 3000);
		if (ret < 0)
		{
			perror("epoll_wait");
			return 0;
		}
		else if (ret == 0)
		{
			printf("timeout\n");
			continue;
		}

		for (int i = 0; i < ret; ++i)
		{
			if (fd_arr[i].data.fd == 0)
			{
				char buf[1024] = {0};
				read(fd_arr[i].data.fd, buf, sizeof(buf) - 1);
				printf("buf is %s\n", buf);
			}
		}
	}
    return 0;
}

