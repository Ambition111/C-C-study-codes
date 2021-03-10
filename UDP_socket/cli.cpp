#include "udpser.hpp"
#include <stdio.h>
#include <stdlib.h>


//对于客户端程序而言，命令行参数当中的ip和port需要指定为服务端的ip和port

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "./cli [ser_ip] [ser_port]" << std::endl; 
        return 0;
    }

    std::string ser_ip = argv[1];
    uint16_t ser_port = atoi(argv[2]);

    UdpSer us;

    if (!us.CreateSocket())
        return 0;


    //组织服务端的地址信息，以便Send使用
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;     //采用ipv4
    dest_addr.sin_port = htons(ser_port);   //将端口的本机字节序转换为网络字节序
    dest_addr.sin_addr.s_addr = inet_addr(ser_ip.c_str());  //将string类型的ip地址转换为uint16_t,并将它的本机字节序转换为网络字节序

    while (1)
    {
        //1.发送数据
        std::cout << "cli say:";
        fflush(stdout);
        std::string buf;
        std::cin >> buf;

        us.Send(buf, &dest_addr);
        //2.接收数据
        struct sockaddr_in peeraddr;
        us.Recv(&buf, &peeraddr);

        std::cout << "ser say:" << buf.c_str() << std::endl;
    }

    us.Close();

    return 0;
}

