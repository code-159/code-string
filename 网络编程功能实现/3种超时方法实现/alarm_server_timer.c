#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define MY_NET 		"192.168.0.108"
#define NET_NUMBER 	50005

void handler(int signo)   
{   
	return; 
}

int main(void)
{
	int	 server_fd  	= -1;
	int  file_fd    	= -1;
	char buff[100] 		= {0};
	char file_buff[1024] = {0};
	int  retval ;
	int  temp_fd;
	
	struct sockaddr_in server_addr; // 绑定本机的IP和端口  
	struct sockaddr_in listen_client_addr; // 存放客户端的IP地址
	
	socklen_t addrlen = sizeof(listen_client_addr);//定义监听客户端的长度
		
	// 1.创建套接字 AF_INET 和 PF_INET 是一样的
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd == -1)
	{
		perror("socket create fail!\n");
		return -1;
	}
	
	memset(&server_addr, 0, sizeof(server_addr));//清楚文件里的内容
	
	//2.设置（初始化）本机的IP和端口、协议
	server_addr.sin_family		= AF_INET; /*协议地址*/  
	server_addr.sin_port		= htons(NET_NUMBER); /*将端口号转换为网络字节序*/ 
	server_addr.sin_addr.s_addr	= inet_addr(MY_NET);      /* 绑定的本机IP，0.0.0.0代表本机所有IP htonl(INADDR_ANY) 代表本机所有IP*/

	// 3.绑定本机端口和IP地址、协议
	retval = bind (server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if(retval == -1)
	{
		perror("bind error\n");
		return -1;
	}
	printf("服务端绑定成功\n");
	
	// 4. 设置监听 DDOS
	listen(server_fd, 5); // 同时可以接收5个连接请求

	//5.开始监听，等待客户端链接 阻塞函数
	memset(&listen_client_addr, 0, sizeof(listen_client_addr));
	temp_fd = accept(server_fd, (struct sockaddr *)&listen_client_addr, &addrlen);
	if(temp_fd == -1)
	{
		perror("create accept error\n");
		return -1;
	}
	
	printf("客户端连接成功\n");
	printf("客户端的地址：%s\n", inet_ntoa(listen_client_addr.sin_addr));
	printf("客户端的端口：%d\n", ntohs(listen_client_addr.sin_port));
	
	struct sigaction  act;
	
	while(1)
	{
		//setsockopt(temp_fd, SOL_SOCKET, SO_RCVTIMEO, (const void *)&ts, sizeof(ts));
		
		sigaction(SIGALRM, NULL, &act); // 获取信号处理方法到act
		act.sa_handler = handler; 		// 设置如果收到了SIGALRM信号，就执行handler函数
		act.sa_flags &= ~SA_RESTART; 	// 清空标志位
		sigaction(SIGALRM, &act, NULL); // 将刚刚修改的设置到内核
		alarm(3); 						// 3秒之后给本进程发送一个SIGALRM信号
		
		// 收发消息
		memset(buff, 0, sizeof(buff));//先清空buff中的内容
		
		recv(temp_fd, buff, sizeof(buff),0);
		
		printf("服务器收到消息:%s\n",buff);
	}

	close(server_fd);
	close(temp_fd);

	
	return 0;
}