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

int	g_temp_fd1 = -1;
int g_client_fd = -1;

int main(void)
{
	char		offsetbuf[32];
	int			listen_fd	= -1; 
	pthread_t	tid			= 0;
	char		rbuff[1024]	= {0};
	int 		ret			= -1; 
	long 		offset;

	
	struct sockaddr_in   my_addr; // 绑定本机的IP和端口  
	struct sockaddr_in   src_addr; // 存放客户端的IP地址
	
	// 1.创建套接字 AF_INET 和 PF_INET 是一样的
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_fd < 0)
	{
		printf("socket create fail!\n");
		return -1;
	}
	
	printf("字节= %lu\n", sizeof("你好"));
	
	//2.设置（初始化）本机的IP和端口、协议
	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family		= AF_INET; /*协议地址*/  
	my_addr.sin_port		= htons(3389); /*将端口号转换为网络字节序*/ 
	my_addr.sin_addr.s_addr	= inet_addr("0.0.0.0");      /* 绑定的本机IP，0.0.0.0代表本机所有IP INADDR_ANY 代表本机所有IP*/
	
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
		return -2;
	}
	printf("绑定成功\n");
	
	// 4. 设置监听 DDOS
	listen(listen_fd, 5); // 同时可以接收5个连接请求

	socklen_t addrlen = sizeof(src_addr);

	//5.开始监听，等待客户端链接 阻塞函数
	memset(&src_addr, 0, sizeof(src_addr));
	g_client_fd = accept(listen_fd, (struct sockaddr *)&src_addr, &addrlen);
	if(g_client_fd < 0)
	{
		perror("accept");
		return -2;
	}
	printf("客户端连接成功\n");
	printf("客户端的地址：%s\n", inet_ntoa(src_addr.sin_addr));
	printf("客户端的端口：%d\n", ntohs(src_addr.sin_port));

again:
	// 首先接收服务器发送的文件名
	recv(g_client_fd, rbuff, sizeof(rbuff), 0);


	// 打开文件，如果打不开说明不存在，回复没有这个文件，如果能打开，说明文件存在，回复文件存在
	int file_fd = open(rbuff, O_RDONLY);
	// 判断是否打开成功
	if(file_fd < 0)
	{
		perror("open");

		send(g_client_fd, "fileNoLive", sizeof("fileNoLive"), 0);
		goto again;
	}
	printf("文件打开成功\n");

	// 如果打不开说明不存在，回复没有这个文件，如果能打开，说明文件存在，回复文件存在
	send(g_client_fd, "fileIsLive", sizeof("fileIsLive"), 0); // 回复文件是否存在
	
	// 接收客户端发过来的文件的字节数，根据这个字节数偏移文件指针
	recv(g_client_fd, rbuff, sizeof(rbuff), 0); // 接收文件字节数
	
	strncpy(offsetbuf, rbuff, 32);
	offset = atoi(offsetbuf);
	printf("offset = %ld\n", offset);

	lseek(file_fd, offset, SEEK_SET); // 偏移文件指针
	
	// 循环读取数据并发送
	while(1)
	{
		// 读取文件内容，一次读取100字节
		ret = read(file_fd, rbuff, sizeof(rbuff));
		//printf("读取了%d字节\n", ret);
		if(ret == 0)
		{
			usleep(10000);
			// 发送结束标志
			
			//ret = send(g_client_fd, "end", 3, 0);
			//printf("发送了%d字节\n", ret);
			printf("服务端发送数据完成!\n");
			
			close(file_fd);
			
			// 接收客户端的回复，确保客户端已经接收完成
			
			break;
		}
		
		else if(ret > 0)
		{
			// 发送文件数据，一次发送100字节
			usleep(100);
			ret = send(g_client_fd, rbuff, ret, 0);
			//printf("发送了%d字节\n", ret);
		}
		
		// 解决的问题:
		// 如何判断读取完了？ 可以通过文件属性、读取的字节数小于100判断
		// 接收方（客户端）如何判断收完了？ 如果发送方知道文件已经传输完成，那么在最后可以发送一个标志（字符串），例如end; ,建议传输完成后，延时一段时间再发end
	}
	

	close(listen_fd);
	//close(g_temp_fd);
	
	return 0;
}