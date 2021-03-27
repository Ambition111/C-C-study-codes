/*====================================================
*   Copyright (C) 2021  All rights reserved.
*
*   文件名称：main.cpp
*   创 建 者：phw
*   创建日期：2021年03月23日
*   描    述：
================================================================*/
#include "epoll_et_ser.hpp"

#define CHECK_RET(p) if (p != true) {return 0;}

using namespace std;
int main(int argc, char *argv[])
{
	TcpSer listen_ts;

	CHECK_RET(listen_ts.CreateSocket());
	CHECK_RET(listen_ts.Bind("0.0.0.0", 19999));
	CHECK_RET(listen_ts.Listen());

	EpollSer es;

	es.AddFd(listen_ts.GetFd());

	while (1)
	{
		std::vector<TcpSer> vec;
		if (!es.EpollWait(&vec))
		{
			continue;
		}

		for (ssize_t i = 0; i < vec.size(); ++i)
		{
			if (vec[i].GetFd() == listen_ts.GetFd())
			{
				TcpSer peerts;
				struct sockaddr_in peeraddr;
				listen_ts.Accept(&peeraddr, &peerts);

				printf("Have a new connection %s : %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

				peerts.SetNonBlock();

				//true:设置为ET模式
				es.AddFd(peerts.GetFd(), true);
			}

			else
			{
				std::string buf;
				bool ret = vec[i].RecvNonBlock(&buf);
				if (!ret)
				{
					es.DeleteFd(vec[i].GetFd());
					vec[i].Close();
					continue;
				}

				printf("recv data is %s\n", buf.c_str());
				buf.clear();
				buf.assign("I am server");
				vec[i].SendNonBlock(buf);
			}

		}

	}
	
	listen_ts.Close();
    return 0;
}

