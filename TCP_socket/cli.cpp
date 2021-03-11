#include "tcpser.hpp"


//通过main函数的命令行参数获取连接服务端的ip和port
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Start Client need put：./cli [ip] [port]" << std::endl;
        return 0;
    }

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSer ts;

    if (!ts.CreateSocket())     //创建套接字失败
        return 0;

    if (!ts.Connect(ip, port))  //连接服务端失败
        return 0;

    while (1)
    {
        std::cout << "please enter a message to server：";
        fflush(stdout);

        std::string buf;
        std::cin >> buf;

        ts.Send(buf);

        //服务端没有返回数据，意味着客户端TCPsocket套接字中的接收缓冲区中没有数据，
        //没有数据可以接收，即recv的flags为0的情况下，则recv接口阻塞等待
        ts.Recv(&buf);
        std::cout << "server say：" << buf << std::endl;
    }

    ts.Close();


    return 0;
}
