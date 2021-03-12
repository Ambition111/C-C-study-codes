#include "tcpser.hpp"
#include <signal.h>
#include <sys/wait.h>

void sigcb(int signo)
{
    (void)signo;
    while (1)
    {
        waitpid(-1, NULL, WNOHANG);
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "start server: ./ser [ip] [port]" << std::endl;
        return 0;
    }

    signal(SIGCHLD, sigcb);

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
        TcpSer newts;   //保存新创建出来的套接字的对象

        struct sockaddr_in peeraddr;

        if (!ts.Accept(&peeraddr, &newts))
            continue;
        
        //inet_ntoa():将uint16_t的整数转换为点分十进制的ip地址，并将网络字节序转换为主机字节序
        //ntohs():将port的网络字节序转换为主机字节序
        std::cout << "Have a new connection ip = " << inet_ntoa(peeraddr.sin_addr) << "," << ntohs(peeraddr.sin_port) << std::endl;

        int pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(1);
        }
        else if (pid == 0)
        {//child
            while (1)
            {
                std::string buf;
                newts.Recv(&buf);

                std::cout << "client did say:" << buf.c_str() <<std::endl;
                std::cout << "please return a message to client:";
                fflush(stdout);

                std::cin >> buf;
                newts.Send(buf);
            }
            
            newts.Close();
            exit(1);
        }
        else
        {//father
            //waitpid();
            newts.Close();
        }
    }

    return 0;
}


