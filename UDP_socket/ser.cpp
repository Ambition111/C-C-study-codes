#include "udpser.hpp"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    //  ./ser [ip] [port]
    if (argc != 3)
    { 
        std::cout << "Use this process, please: ./ser [ip] [port]" << std::endl;
        return 0;
    }
    //通过main函数的命令行参数，得到想要监听的ip和端口
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    UdpSer us;

    if (!us.CreateSocket())
    {
        return 0;
    }
    if (!us.Bind(ip, port))
        return 0;

    while (1)
    {
        //接收数据
        std::string buf;
        struct sockaddr_in peeraddr; //peeraddr:对端地址信息
        us.Recv(&buf, &peeraddr);
        std::cout << "cli say:" << buf.c_str() << std::endl;

        //发送数据
        std::cout << "server say:";
        fflush(stdout);
        std::cin >> buf;
        us.Send(buf, &peeraddr);
    }

    us.Close();

    return 0;
}
