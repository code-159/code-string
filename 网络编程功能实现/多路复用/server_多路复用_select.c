#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MY_NET	    "192.168.0.108"
#define NET_NUMBER 	50005

int	g_client_fd	=	-1;

int	main(void){
	int			listen_fd	= -1; 
	char		rbuff[100]	= {0};
	int 		ret			= -1; 
	int			sec_fd_max	= -1;	
	int 		client_fds[20]	= {0};
	fd_set		rfdset; // 文件描述符集合
	
	struct sockaddr_in   my_addr; // 绑定本机的IP和端口  
	struct sockaddr_in   src_addr; // 存放客户端的IP地址
	
	// 1.创建套接字 AF_INET 和 PF_INET 是一样的
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_fd < 0)
	{
		printf("socket create fail!\n");
		return -1;
	}
	
	//2.设置（初始化）本机的IP和端口、协议
	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family		= AF_INET; /*协议地址*/  
	my_addr.sin_port		= htons(NET_NUMBER); /*将端口号转换为网络字节序*/ 
	my_addr.sin_addr.s_addr	= inet_addr("127.0.0.1");      /* 绑定的本机IP，0.0.0.0代表本机所有IP INADDR_ANY 代表本机所有IP*/
	
	// 设置端口允许重复绑定
	int opt = 1;
	// sockfd为需要端口复用的套接字
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
	
	// 3.绑定本机端口和IP地址、协议
	int status = bind (listen_fd, (struct sockaddr *) &my_addr, sizeof(my_addr));
	// 判断绑定是否成功
	if(status < 0)
	{
		perror("band");
		return -1;
	}
	printf("绑定成功\n");
	
	// 4. 设置监听 DDOS
	listen(listen_fd, 5); // 同时可以接收5个连接请求

	socklen_t addrlen = sizeof(src_addr);
	
	while(1)
	{
		// 清空集合
		FD_ZERO(&rfdset);         // 清空集合
		FD_SET(listen_fd, &rfdset); // 将fd添加到集合里面
		sec_fd_max = listen_fd; // 假设监听套接字是最大值
		
		// 寻找当前需要查询的文件描述符里面的最大值
		for(int i=0; i<20; i++)
		{
			if(client_fds[i] > 0)//大于0就证明这个文件描述符里有数据，可监听
			{
				FD_SET(client_fds[i], &rfdset); // 将fd添加到集合里面
				sec_fd_max = sec_fd_max > client_fds[i] ? sec_fd_max:client_fds[i];// 找出当前客户端里的文件描述符的最大值
			}
		}
		// 使用select多路复用查询是否有客户端连接,且为阻塞函数，没有客户端进入时，会一直阻塞
		select(sec_fd_max + 1, &rfdset, NULL, NULL, NULL);
		
		// 如果有客户端连接，listen_fd里就有数据，我们可以处理（accept）
		if(FD_ISSET(listen_fd, &rfdset)) // 判断listen_fd是否还在集合里面
		{
			//5.开始监听，等待客户端链接 阻塞函数
			memset(&src_addr, 0, sizeof(src_addr));
			g_client_fd = accept(listen_fd, (struct sockaddr *)&src_addr, &addrlen);
			if(g_client_fd < 0)
			{
				perror("accept");
				return -1;
			}
			printf("客户端连接成功\n");
			printf("g_client_fd = %d\n", g_client_fd);
			printf("客户端的地址：%s\n", inet_ntoa(src_addr.sin_addr));
			printf("客户端的端口：%d\n", ntohs(src_addr.sin_port));
			
			for(int i=0; i<20; i++)//这个for循环是为了把新连接的客户端的文件标识符存到集合里
			{
				if(client_fds[i] == 0)
				{
					client_fds[i] = g_client_fd;
					break;
				}
			}
		}
		else
		{
			//查询有无客户端通信
			for(int i=0; i<20; i++)/*
										这个for是判断当被踢出去的文件描述符发送信息给服务的时，
										也就是这个文件描述符被函数FD_ZERO(&rfdset)清空了,
										判断如果客户端发送数据则接收并读取出来。
									*/
			{
				// 如果该客户端的文件描述符还在集合里，说明有数据可以接收
				if(FD_ISSET(client_fds[i], &rfdset))
				{
					memset(rbuff, 0, sizeof(rbuff));
					ret = recv(client_fds[i], rbuff, sizeof(rbuff), 0);
					
					printf("ret = %d\n", ret);
					// 当客户端断开时，先关闭它再把其至0
					if(ret <= 0)
					{
						printf("客户端%d断开服务器\n",client_fds[i]);
						close(client_fds[i]);
						client_fds[i] = 0;
						
						break;
					}
					printf("%d:%s\n", client_fds[i], rbuff);
				}
			}
			
		}
		
	}
	close(listen_fd);
	
	return 0;
}