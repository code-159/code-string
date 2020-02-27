#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MY_NET "192.168.0.108"


int main(void)
{
	int		client_fd	= -1;
	char	buff[100]	= {0};
	int 	retval;
	
	struct 	sockaddr_in	server_addr; // 绑定本机的IP和端口  
	socklen_t addrlen = sizeof(server_addr);
	
	//1.创建套接字 AF_INET 和 PF_INET 是一样的
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd == -1)
	{
		perror("socket create fail!\n");
		return -1;
	}
	
	memset(&server_addr, 0, sizeof(server_addr));
	
	/*2.设置（初始化）本机的IP和端口、协议
		这里是填写需要连接服务端的信息，就想当于送货上门需要知道的地址，
		而不需要我们自己的地址，客户端就是快递员，只需要知道服务端的信息就行
	*/

	server_addr.sin_family		= AF_INET; /*协议地址*/  
	server_addr.sin_port		= htons(50005); /*将端口号转换为网络字节序*/ 
	server_addr.sin_addr.s_addr	= inet_addr(MY_NET);  

	//3.连接上了服务器，connect函数三个参数
	retval = connect(client_fd, (struct sockaddr *)&server_addr, addrlen);
	if(retval == -1)
	{
		perror("connect server error\n");
		return -1;
		
	}

	printf("连接上了服务器\n");
	
	while(1)
	{
		memset(buff, 0, sizeof(buff));
		
		// 收发消息
		scanf("%s", buff);
		
		send(client_fd, buff, 100,0);
		//send函数和write函数类似，就是多了一个参数，且一般为0
		printf("正在向服务端发送信息：%s\n", buff);
	}
	
	close(client_fd);
	
	return 0;
}