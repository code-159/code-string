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

int	g_socket_fd	= -1;

int main(void)
{
	
	char	wbuff[100]	= {0};
	struct sockaddr_in   server_addr; // 绑定本机的IP和端口  

	
	// 1.创建套接字 AF_INET 和 PF_INET 是一样的
	g_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(g_socket_fd < 0)
	{
		printf("socket create fail!\n");
		return -1;
	}
	
	//2.设置（初始化）本机的IP和端口、协议
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family		= AF_INET; /*协议地址*/  
	server_addr.sin_port		= htons(50004); /*将端口号转换为网络字节序*/ 
	server_addr.sin_addr.s_addr	= inet_addr("127.0.0.1"); // 120.25.226.148     
	socklen_t addrlen = sizeof(server_addr);
	int ret = connect(g_socket_fd, (struct sockaddr *)&server_addr, addrlen);
	if(ret < 0)
	{
		perror("connect\n");
		return -2;
		
	}

	printf("连接上了服务器\n");
	
	
	const char* OOB_DATA = "ab"; // 带外数据
	const char* NORMAL_DATA = "123"; // 传输的数据
	
	//
	while(1)
	{

		
		// 发送123
		ret = send(g_socket_fd, NORMAL_DATA, strlen(NORMAL_DATA),0); 
		printf("发送了%d\n", ret);
		
		// 发送带外数据
		ret = send(g_socket_fd, OOB_DATA, strlen(OOB_DATA),MSG_OOB); 
		printf("发送了%d\n", ret);
		
		// 发送123
		ret = send(g_socket_fd, NORMAL_DATA, strlen(NORMAL_DATA),0); 
		printf("发送了%d\n", ret);
		
		sleep(3);
		// send
		//printf("rbuf = %s\n", wbuff);
	}

	return 0;
}