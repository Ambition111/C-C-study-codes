#include "tcpser.hpp"


int main(int argc, char* argv[])
{   
    if (argc != 3)  //参数不够，需要输入ip和port
    {
        std::cout << "Start Server need put：./ser [ip] [port]" <<std::endl;
        return 0;
    }
    
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSer ts;
    if (!ts.CreateSocket()) //创建套接字失败
        return 0;

    if (!ts.Bind(ip, port)) //绑定地址信息失败
        return 0;

    if (!ts.Listen())       //监听失败
        return 0;

    //为accept函数接收回来的新创建的socket套接字准备的
    TcpSer ser_ts;

    struct sockaddr_in client_addr;
    if (!ts.Accept(&client_addr, &ser_ts)) //获取连接失败
        return 0;

    while (1)
    {
        //如果只调用一次accept，就只能接收一个客户端的连接
        //如果将accept放入while循环里面，则进行一次收发数据的时候，都需要阻塞等待新连接的到来
        /*
        struct sockaddr_in client_addr;
        if (!ts.Accept(&client_addr, &ser_ts)) //获取连接失败
            return 0;
        */
        std::string buf;
        ser_ts.Recv(&buf);
        std::cout << "client say：" << buf.c_str() <<std::endl;
        std::cout << "please return a message to client：";
        fflush(stdout);
        std::cin >> buf;

        ser_ts.Send(buf);
    }

    ser_ts.Close(); //关闭新创建的套接字
    ts.Close();     //关闭监听套接字

    return 0;
}

