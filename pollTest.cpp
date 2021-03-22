#include <iostream>
#include <poll.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;
int main(int argc, char *argv[])
{
	//1.�����¼��ṹ����
	struct pollfd fd_arr[10];
	fd_arr[0].fd = 0;
	fd_arr[0].events = POLLIN;

	//2.���
	while (1)
	{
		int ret = poll(fd_arr, 1, 1000);
		if (ret < 0)
		{
			perror("poll");
			return -1;
		}
		else if (ret == 0)
		{
			printf("timeout\n");
			continue;
		}

		//2.1 �������ɶ��¼�ʱ�������¼�
		for (int i = 0; i < ret; i++)
		{
			if (fd_arr[i].revents == POLLIN)
			{
				char buf[1024] = {0};
				read(fd_arr[i].fd, buf, sizeof(buf) - 1);
				printf("read content is = %s\n", buf);
			}
		}
	}
    return 0;
}

