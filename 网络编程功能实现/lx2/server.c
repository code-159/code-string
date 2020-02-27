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

#define MY_NET "192.168.0.108"

int temp_fd_1  = -1;
int temp_fd_2  = -1;
char buff[100] = {0};

void *tcp_send_msg(void *arg)
{
	while(1)
	{
		// 收发消息
		memset(buff, 0, sizeof(buff));//先清空buff中的内容
		
		//读到2
		recv(temp_fd_2, buff, 100, 0);
		//recv函数与read函数类似，就是多了一个参数，且大部分都为0
		printf("接受到客户端2信息为: %s  ", buff);
		
		//发给1
		send(temp_fd_1, buff, 100,0);
		//send函数和write函数类似，就是多了一个参数，且一般为0
		printf("正在向客户端1发送信息\n");
	}
}

int main(void)
{
	int		server_fd	= -1;
	
	
	int 	retval;
	
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
	server_addr.sin_port		= htons(50005); /*将端口号转换为网络字节序*/ 
	server_addr.sin_addr.s_addr	= inet_addr(MY_NET);      /* 绑定的本机IP，0.0.0.0代表本机所有IP htonl(INADDR_ANY) 代表本机所有IP*/

	// 3.绑定本机端口和IP地址、协议
	retval = bind (server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if(retval == -1)
	{
		perror("bind error\n");
		return -1;
	}
	
	// 4. 设置监听 DDOS
	listen(server_fd, 5); // 同时可以接收5个连接请求

	//5.开始监听，等待客户端链接 阻塞函数
	memset(&listen_client_addr, 0, sizeof(listen_client_addr));
	temp_fd_1 = accept(server_fd, (struct sockaddr *)&listen_client_addr, &addrlen);
	if(temp_fd_1 == -1)
	{
		perror("create accept error\n");
		return -1;
	}

	printf("第一个客户端连接\n");

	temp_fd_2 = accept(server_fd, (struct sockaddr *)&listen_client_addr, &addrlen);
	if(temp_fd_2 == -1)
	{
		perror("create accept error\n");
		return -1;
	}
	printf("第二个客户端连接\n");
	
	//创建线程，接收数据
	pthread_t tid;
	errno = pthread_create(&tid, NULL, tcp_send_msg, NULL);
	if(errno != 0)
	{
		perror("creat thread error");
		return -1;
	}
	
	while(1)
	{
		// 收发消息
		memset(buff, 0, sizeof(buff));//先清空buff中的内容
		
		//读到1
		recv(temp_fd_1, buff, 100, 0);
		//recv函数与read函数类似，就是多了一个参数，且大部分都为0
		printf("接受到客户端1信息为: %s  ", buff);
		
		//发给2
		send(temp_fd_2, buff, 100,0);
		//send函数和write函数类似，就是多了一个参数，且一般为0
		printf("正在向客户端2发送信息\n");
	}

	retval = close(server_fd);
	if(retval == -1)
	{
		perror("close server_fd error\n");
		return -1;
	}
	
	retval = close(temp_fd_1);
	if(retval == -1)
	{
		perror("close temp_fd error\n");
		return -1;
	}
	close(temp_fd_2);
	
	return 0;
}