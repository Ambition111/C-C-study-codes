#include "epoll_lt_ser.hpp"
#define CHECK_RET(p) if (p != true) {return -1;}

int main()
{
    TcpSer listen_ts;

    CHECK_RET(listen_ts.CreateSocket());
    CHECK_RET(listen_ts.Bind("0.0.0.0", 19997));
    CHECK_RET(listen_ts.Listen());

	EpollSer es;
	CHECK_RET(es.InitSer());
	es.AddFd(listen_ts.GetFd());	//添加侦听套接字


    while (1)
    {
        //1.监控
        std::vector<TcpSer> vec;    //将要监控的描述符存到一个vector里
        if (!es.EpollWait(&vec))
        {
            continue;
        }
        for (size_t i = 0; i < vec.size(); ++i)
        {
            //2.接收新的连接
            if (listen_ts.GetFd() == vec[i].GetFd())    //vec里的文件描述符为侦听套接字
            {
                struct sockaddr_in peeraddr;
                TcpSer peerts;
                listen_ts.Accept(&peeraddr, &peerts);
                printf("Have a new connection : [ip] : %s [port] : %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

                es.AddFd(peerts.GetFd());
            }
            //3.连接上有数据
            else                        //vec里的文件描述符为新创建的套接字
            {
                std::string data;
                bool ret = vec[i].Recv(&data);
                if (!ret)
                {
                    es.DeleteFd(vec[i].GetFd());
                    vec[i].Close();
                }
                vec[i].Recv(&data);
                printf("client send data is [%s]\n", data.c_str());
            }
        }
    }
    return 0;
}
