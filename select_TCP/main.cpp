#include "SelectSer.hpp"
#define CHECK_RET(p) if (p != true) {return -1;}

int main()
{
    TcpSer listen_ts;

    CHECK_RET(listen_ts.CreateSocket());
    CHECK_RET(listen_ts.Bind("0.0.0.0", 19997));
    CHECK_RET(listen_ts.Listen());

    SelectSer ss;
    ss.AddFd(listen_ts.GetFd());    //添加侦听套接字到事件集合中

    while (1)
    {
        //1.监控
        std::vector<TcpSer> vec;    //将要监控的描述符存到一个vector里
        if (!ss.SelectWait(&vec))
        {
            continue;
        }
        for (size_t i = 0; i < vec.size(); ++i)
        {
            //2.接收新的连接
            if (listen_ts.GetFd() == vec[i].GetFd())    //监控的为侦听套接字
            {
                struct sockaddr_in peeraddr;
                TcpSer peerts;
                listen_ts.Accept(&peeraddr, &peerts);
                printf("Have a new connection : [ip] : %s [port] : %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

                //将新创建出来的套接字添加到select的事件集合中去
                ss.AddFd(peerts.GetFd());
            }
            //3.连接上有数据
            else                        //监控的为新创建的套接字
            {
                std::string data;
                bool ret = vec[i].Recv(&data);
                if (!ret)
                {
                    ss.DeleteFd(vec[i].GetFd());
                    vc[i].Close();
                }
                vec[i].Recv(&data);
                printf("client send data is [%s]\n", data.c_str());
            }
        }
    }
    return 0;
}
