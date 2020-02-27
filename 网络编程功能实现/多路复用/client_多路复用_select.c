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

int	client_fd	= -1;

int main(void)
{
	char	buffer[100]	= {0};
	char    arr[100]	= {0};
	struct  sockaddr_in   server_addr; // 绑定本机的IP和端口  
	
	// 1.创建套接字 AF_INET 和 PF_INET 是一样的
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd < 0)
	{
		printf("socket create fail!\n");
		return -1;
	}
	
	//2.设置（初始化）本机的IP和端口、协议
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family		= AF_INET; /*协议地址*/  
	server_addr.sin_port		= htons(NET_NUMBER); /*将端口号转换为网络字节序*/ 
	server_addr.sin_addr.s_addr	= inet_addr("127.0.0.1"); // 120.25.226.148  
	
	socklen_t addrlen = sizeof(server_addr);
	
	int ret = connect(client_fd, (struct sockaddr *)&server_addr, addrlen);
	if(ret < 0)
	{
		perror("connect\n");
		return -1;
	}

	printf("连接上了服务器\n");
	
	char *ptr = "马飞飞：";
	
	while(1)
	{
		memset(buffer, 0, sizeof(buffer));
		memset(arr, 0, sizeof(arr));
		printf("请输入信息：");
		scanf("%s", buffer);
		sprintf(arr , "%s%s", ptr , buffer);

		send(client_fd, arr, strlen(arr),0);
		
		printf("正在给客户端发消息：%s\n",buffer);
	}

	return 0;
}