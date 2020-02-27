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

//主播程序，只向服务端发送数据，服务端在分发数据给没个客户端

#define MY_NET	    "192.168.0.108"
#define NET_NUMBER 	50005


int main(void)
{
	char buff[100]	= {0};
	int  retval;
	int	 big_man_fd	= -1;
	
	struct  sockaddr_in  big_man_addr; // 绑定本机的IP和端口  
	socklen_t  addrlen = sizeof(big_man_addr);
	
	//1.创建套接字 AF_INET 和 PF_INET 是一样的
	big_man_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(big_man_fd == -1)
	{
		perror("socket create fail!\n");
		return -1;
	}
	
	memset(&big_man_addr, 0, sizeof(big_man_addr));//清空内容
	
	/*2.设置（初始化）本机的IP和端口、协议
		这里是填写需要连接服务端的信息，就想当于送货上门需要知道的地址，
		而不需要我们自己的地址，客户端就是快递员，只需要知道服务端的信息就行
	*/
	
	big_man_addr.sin_family		= AF_INET; /*协议地址*/  
	big_man_addr.sin_port		= htons(NET_NUMBER); /*将端口号转换为网络字节序*/ 
	big_man_addr.sin_addr.s_addr	= inet_addr(MY_NET);  

	//3.连接上了服务器，connect函数三个参数
	retval = connect(big_man_fd, (struct sockaddr *)&big_man_addr, addrlen);
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
		
		send(big_man_fd, buff, 100,0);
		//send函数和write函数类似，就是多了一个参数，且一般为0
		printf("正在发送信息：%s\n", buff);
	}
	
	close(big_man_fd);
	
	return 0;
}