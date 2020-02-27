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


int main(void)
{
	long 	offset_size;
	int 	file_fd;
	int		g_socket_fd	= -1;
	char	wbuff[1024]	= {0};
	char	rbuff[1024]	= {0};
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
	server_addr.sin_port		= htons(3389); /*将端口号转换为网络字节序*/ 
	server_addr.sin_addr.s_addr	= inet_addr("127.0.0.1"); // 120.25.226.148     
	socklen_t addrlen = sizeof(server_addr);
	int ret = connect(g_socket_fd, (struct sockaddr *)&server_addr, addrlen);
	if(ret < 0)
	{
		perror("connect\n");
		return -1;
		
	}

	printf("连接上了服务器\n");
	
	// 客户端发送文件名字给服务器
	printf("请输入要下载的文件名:");
	scanf("%s", wbuff);
	send(g_socket_fd, wbuff, sizeof(wbuff), 0);
	
	// 接收服务器回复的消息
	recv(g_socket_fd, rbuff, sizeof(rbuff), 0);

	//判断rbuff，代表服务器文件存在还是不存在，如果不存在就退出客户端，如果存在，就创建并打开该文件
	// 判断是否打开成功
	if(strncmp(rbuff, "fileNoLive", sizeof("fileNoLive")) == 0)
	{
		printf("客户端没有该文件!\n");
		return -1;
	}
	
	else if(strncmp(rbuff, "fileIsLive", sizeof("fileIsLive")) == 0)
	{
		file_fd = open(wbuff, O_RDWR | O_CREAT, 0666);
		if(file_fd < 0)
		{
			perror("file_fd");
			return -1;
		}
		printf("文件打开/创建成功\n");
	}

	// 打开文件之后，将文件指针偏移到最后,就是要获取文件当前的大小
	offset_size = lseek(file_fd, 0, SEEK_END); // ftell、stat等等
	printf("offset_size = %ld\n", offset_size);
	// 将当前的文件的字节数发送给客户端

	sprintf(wbuff, "%ld", offset_size);

	send(g_socket_fd, wbuff, sizeof(wbuff), 0); // 有一个函数是将数字的字符串，直接转成数字
	
	// 等待着循环接收数据
	while(1)
	{
		memset(wbuff, 0, sizeof(wbuff));
		ret = recv(g_socket_fd, wbuff, sizeof(wbuff), 0); // 发送时如果数据缓存区再结构体前面，那么最好用sizeof发完结构体
		if(ret == 0)
		{
			//printf("接收了%d\n", ret);
			printf("服务器发送完成，数据接收完成\n");
			break;
		}
		
		//printf("接收了%d\n", ret);

		// 写入文件
		write(file_fd, wbuff, ret);
		
		// send
		//printf("rbuff = %s\n", wbuff);
	}

	return 0;
}