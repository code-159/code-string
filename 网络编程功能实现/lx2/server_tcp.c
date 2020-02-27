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

#define MY_NET 		"192.168.0.108"
#define NET_NUMBER 	50005

//服务器程序，收到主播信息后把信息转发给客户端

int main(void)
{
	int	 server_fd  = -1;
	int  temp_fd[3];//三个客户端，一个主播，两个客户
	char buff[100]  = {0};
	int  retval;
	
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
	
	// 4. 设置监听 DDOS
	listen(server_fd, 5); // 同时可以接收5个连接请求

	//5.开始监听，等待客户端链接 阻塞函数
	memset(&listen_client_addr, 0, sizeof(listen_client_addr));
	temp_fd[0] = accept(server_fd, (struct sockaddr *)&listen_client_addr, &addrlen);
	if(temp_fd[0] == -1)
	{
		perror("create accept error\n");
		return -1;
	}
	printf("主播客户端连接\n");

	temp_fd[1] = accept(server_fd, (struct sockaddr *)&listen_client_addr, &addrlen);
	if(temp_fd[1] == -1)
	{
		perror("create accept error\n");
		return -1;
	}
	printf("第一个客户端连接\n");

	temp_fd[2] = accept(server_fd, (struct sockaddr *)&listen_client_addr, &addrlen);
	if(temp_fd[2] == -1)
	{
		perror("create accept error\n");
		return -1;
	}
	printf("第二个客户端连接\n");
	
	while(1)
	{
		// 收发消息
		memset(buff, 0, sizeof(buff));//先清空buff中的内容
		
		//读到主播的数据
		recv(temp_fd[0], buff, 100, 0);
		//recv函数与read函数类似，就是多了一个参数，且大部分都为0
		printf("接受到主播的信息为: %s \n", buff);
		
		//发给客户端一和二
		for(retval = 1 ; retval < 3 ; retval++ )
		{
			send(temp_fd[retval], buff, 100 , 0);
			printf("正在向客户端 %d 发送信息\n",retval);
			usleep(10000);
		}
		//send函数和write函数类似，就是多了一个参数，且一般为0
	}

	close(temp_fd[0]);
	close(temp_fd[1]);
	close(temp_fd[2]);
	
	return 0;
}