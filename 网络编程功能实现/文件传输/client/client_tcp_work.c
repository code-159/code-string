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

//客户端程序，发送lbwNB时，服务端传一个文件过来

#define MY_NET	    "192.168.0.108"
#define NET_NUMBER 	50005

int	 client_fd	= -1;

void *tcp_recv_msg(void *arg)
{
	char arr[1024] = {0};
	int file_fd   = -1;
	int retval    = -1;
	
	usleep(10000);
	//printf("正在接收文件......\n");
	
	file_fd = open("hero.mp3",O_WRONLY | O_TRUNC | O_CREAT ,0666);
	if(file_fd == -1)
	{
		perror("open file error\n");
		return NULL;
	}
	// 收消息
	while(1)
	{
		memset(arr, 0, sizeof(arr));//先清空buff中的内容
		retval = recv(client_fd, arr, sizeof(arr) , 0);
		printf("接收了%d字节\n",retval);
		//printf("retval = %d\n",retval);
		if(strcmp(arr , "LBW") == 0)
		{
			printf("服务器传输数据完成\n");
			
			close(file_fd);
			break;
		}
		
		retval = write(file_fd , arr , sizeof(arr));
		if(retval == -1)
		{
			perror("write file error\n");
			return NULL;
		}
	}

	printf("完成下载！\n");
}

int main(void)
{
	char buff[100]	= {0};
	int  retval;
	
	struct  sockaddr_in  client_addr; // 绑定本机的IP和端口  
	socklen_t  addrlen = sizeof(client_addr);
	
	//1.创建套接字 AF_INET 和 PF_INET 是一样的
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd == -1)
	{
		perror("socket create fail!\n");
		return -1;
	}
	
	memset(&client_addr, 0, sizeof(client_addr));//清空内容
	
	/*2.设置（初始化）本机的IP和端口、协议*/
	client_addr.sin_family		= AF_INET; /*协议地址*/  
	client_addr.sin_port		= htons(NET_NUMBER); /*将端口号转换为网络字节序*/ 
	client_addr.sin_addr.s_addr	= inet_addr(MY_NET);  

	//3.连接上了服务器，connect函数三个参数
	retval = connect(client_fd, (struct sockaddr *)&client_addr, addrlen);
	if(retval == -1)
	{
		perror("connect server error\n");
		return -1;
		
	}
	printf("连接上了服务器\n");
	
	//创建线程，接收数据
	pthread_t tid;
	errno = pthread_create(&tid, NULL, tcp_recv_msg, NULL);
	if(errno != 0)
	{
		perror("creat thread error");
		return -1;
	}

	while(1)
	{
		memset(buff, 0, sizeof(buff));
		
		// 收发消息
		scanf("%s", buff);
		
		send(client_fd, buff, 100,0);
		//send函数和write函数类似，就是多了一个参数，且一般为0
		printf("正在发送信息：%s\n", buff);
	}
	
	close(client_fd);
	
	return 0;
}