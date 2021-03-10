#pragma once

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


class UdpSer
{
public:
    UdpSer()
    {
        _sock_fd = -1;
    }
    ~UdpSer()
    {

    }
public:
    //1.socket
    bool CreateSocket()
    {
        _sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (_sock_fd < 0)
        {
            perror("socket");
            return false;
        }
        return true;
    }
    //2.bind
    bool Bind(std::string& ip, uint16_t port = 9999)
    {
        struct sockaddr_in addr;    //对于ipv4，是sockaddr_in结构体
        addr.sin_family = AF_INET;  //填充地址域
        addr.sin_port = htons(port);//将端口的主机字节序转换为网络字节序并传入

        //inet_addr():1.将点分十进制的ip转换成uint32_t
        //            2.将主机字节序转换为网络字节序
        addr.sin_addr.s_addr = inet_addr(ip.c_str());

        int ret = bind(_sock_fd, (struct sockaddr*)&addr, sizeof(addr));
        if(ret < 0)
        {
            perror("bind");
            return false;
        }
        return true;
    }
    //3.send
    bool Send(std::string& data, struct sockaddr_in* dest_addr)
    {
        int sendsize = sendto(_sock_fd, data.c_str(), data.size(), 0, (struct sockaddr*)dest_addr, sizeof(struct sockaddr_in));
        if (sendsize < 0)
        {
            perror("sendto");
            return false;
        }
        return true;
    }
    //4.recv
    //buf:出参，返回给调用者接收到了什么数据
    //srcaddr:这条数据从哪里来的，地址信息是数据源端主机的地址信息
    bool Recv(std::string* buf, struct sockaddr_in* src_addr)
    {
        char tmp[1024];
        memset(tmp, '\0', sizeof(tmp));
        socklen_t addrlen = sizeof(struct sockaddr_in);
        
        //如果接收缓冲区当中没有数据，且flags为0，则recvfrom会阻塞
        int recvsize = recvfrom(_sock_fd, tmp, sizeof(tmp) - 1, 0, (struct sockaddr*)src_addr, &addrlen);
        if (recvsize < 0)
        {
            perror("recvfrom");
            return false;
        }

        (*buf).assign(tmp, recvsize);   //为字符串分配一个新值，以替换当前内容
        return true;
    }
    //5.close
    void Close()
    {
        close(_sock_fd);
    }
private:
    int _sock_fd;

};


