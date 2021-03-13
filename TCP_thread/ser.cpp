#include "tcpser.hpp"
#include <pthread.h>

//拿到新创建的sockset和客户端进行TCP通信
void* thread_fun(void* arg)
{
    //线程函数一开始，就将自己与主线程分离。这样主线程就不用等待子线程结束了
    pthread_detach(pthread_self());

    TcpSer* ts = (TcpSer*)arg;
    while (1)
    {
        std::string buf;
        ts->Recv(&buf);
        std::cout << "client did say:" << buf.c_str() << std::endl;
        std::cout << "please return a message to client:";
        fflush(stdout);

        std::cin >> buf;
        ts->Send(buf);
    }
    ts->Close();
    delete ts;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "start server: ./ser [ip] [port]" << std::endl;
        return 0;
    }

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSer ts;

    if (!ts.CreateSocket())
        return 0;

    if (!ts.Bind(ip, port))
        return 0;

    if (!ts.Listen())
        return 0;

    while (1)
    {
        TcpSer* newts = new TcpSer();   //在堆上申请一个指向TcpSer的实例化指针；如果用对象进行操作，出作用域后对象就被释放了，而在线程函数里仍然在使用，就会造成错误。

        struct sockaddr_in peeraddr;

        if (!ts.Accept(&peeraddr, newts))
            continue;
        
        //inet_ntoa():将uint16_t的整数转换为点分十进制的ip地址，并将网络字节序转换为主机字节序
        //ntohs():将port的网络字节序转换为主机字节序
        std::cout << "Have a new connection ip = " << inet_ntoa(peeraddr.sin_addr) << "," << ntohs(peeraddr.sin_port) << std::endl;
        
        pthread_t tid;
        int ret = pthread_create(&tid, NULL, thread_fun, (void*)newts);
        if (ret < 0)
        {
            perror("pthread_create");
            return 0;
        }
    }

    return 0;
}


