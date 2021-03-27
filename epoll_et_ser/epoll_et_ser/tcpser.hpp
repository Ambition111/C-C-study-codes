#pragma once

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>


class TcpSer
{
public:
    TcpSer(int s = -1) : _sockfd(s)
    {
    }
    ~TcpSer()
    {

    }
public:
    //创建套接字
    bool CreateSocket()
    {
        _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    //创建流式套接字
        if (_sockfd < 0)
        {
            perror("socket");
            return false;
        }

        //地址复用
        int i = 1;
        setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(int));
        return true;
    }
    //绑定地址信息
    bool Bind(const std::string& ip, uint16_t port)
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());

        int ret = bind(_sockfd, (struct sockaddr*)&addr, sizeof(addr));
        if (ret < 0)
        {
            perror("bind");
            return false;
        }
        return true;
    }
    //监听
    bool Listen(int backlog = 5)    //默认已连接队列大小为5
    {
        int ret = listen(_sockfd, backlog);
        if (ret < 0)
        {
            perror("listen");
            return false;
        }
        return true;
    }
    //ts：出参，返回一个TcpSer类的实例化指针，在这个指针中保存accpet新创建出来的套接字描述符server_fd
    //    上层调用者可以使用返回的指针和客户端进行通信,因为后面的send和recv都是用这个新的_sockfd操作的
    bool Accept(struct sockaddr_in* peeraddr, TcpSer* ts)
    {
        socklen_t addrlen =  sizeof(struct sockaddr_in);
        int server_fd = accept(_sockfd, (struct sockaddr*)peeraddr, &addrlen);
        if (server_fd < 0)
        {
            perror("accept");
            return false;
        }
        ts->_sockfd = server_fd;
        return true;
    }
    //发起连接（针对client来连接server）
    bool Connect(std::string& ip, uint16_t port)
    {
        struct sockaddr_in dest_addr;
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(port);
        dest_addr.sin_addr.s_addr = inet_addr(ip.c_str());

        int ret = connect(_sockfd, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
        if (ret < 0)
        {
            perror("connect");
            return false;
        }
        return true;
    }
    //发送数据
    bool Send(std::string& data)
    {
        int sendsize = send(_sockfd, data.c_str(), data.size(), 0);
        if (sendsize < 0)
        {
            perror("send");
            return false;
        }
        return true;
    }
    //接收数据
    //data:出参，将接收到的数据返回给调用者
    bool Recv(std::string* data)
    {
        char buf[1024] = {0};
        int recvsize = recv(_sockfd, buf, sizeof(buf) - 1, 0);
        if (recvsize < 0)
        {
            perror("recv");
            return false;
        }
        else if(recvsize == 0)
        {
            std::cout << "peer shutdown connect" << std::endl;
            return false;
        }
        (*data).assign(buf, recvsize);  //将buf内容拷贝到data对象中
        return true;

    }   
    //关闭套接字
    void Close()
    {
        close(_sockfd);
        _sockfd = -1;
    }

public:
    void SetFd(int fd)
    {
        _sockfd = fd;
    }
    int GetFd()
    {
        return _sockfd;
    }

	//非阻塞循环接收
	//buf 是一个出参，保存循环接收到的数据，直到接收缓冲区没有数据了，将buf返还给调用者
	bool RecvNonBlock(std::string* buf)
	{
		while (1)
		{
			//_sockfd不是侦听套接字，而是新连接创建的套接字描述符
			char tmp[3] = {0};
			ssize_t recv_size = recv(_sockfd, tmp, sizeof(tmp) - 1, 0);
			if (recv_size < 0)
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
				{
					//意味着接收缓冲区没有数据了
					break;
				}
				perror("recv");
				return false;
			}
			else if (recv_size == 0)
			{
				//对端关闭掉了连接
				return false;
			}

			//将每次接收到的结果进行累加
			*buf += tmp;

			if (recv_size < (ssize_t)sizeof(tmp) - 1)
			{
				break;
			}
		}
		return true;
	}
	//非阻塞循环发送
	bool SendNonBlock(std::string& buf)
	{
		ssize_t pos = 0;	//发送数据的位置
		ssize_t content_size = buf.size();	//发送剩余数据大小

		while (1)
		{
			ssize_t send_size = send(_sockfd, buf.data() + pos, content_size, 0);
			if (send_size < 0)
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
				{
					continue;
				}
				perror("send");
				return false;
			}

			//更新pos和content_size
			pos += send_size;
			content_size -= send_size;

			if (content_size <= 0)
				break;
		}
		return true;
	}
	//设置文件描述符为非阻塞属性
	void SetNonBlock()
	{
		int flag = fcntl(_sockfd, F_GETFL);
		fcntl(_sockfd, F_SETFL, flag | O_NONBLOCK);
	}
private:
    int _sockfd;
};


