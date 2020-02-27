#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>


#define MY_NET 		"192.168.0.108"
#define NET_NUMBER 	50006

int main(void)
{
	int  client_fd 	= -1;
	int  retval		= -1;
	int  optval 	=  1;
	char buff[100]  = {0};
	
	struct sockaddr_in client_addr; // 绑定本机的IP和端口  
	socklen_t addr_len = sizeof(client_addr);
	
	// 1.创建套接字 AF_INET 和 PF_INET 是一样的
	client_fd = socket(AF_INET, SOCK_DGRAM, 0);  //以UDP的方式来通信
	if(client_fd == -1)
	{
		perror("socket create fail!\n");
		return -1;
	}
	
	//setsockopt(client_fd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &optval, sizeof(int));
	
	memset(&client_addr, 0, sizeof(client_addr));//清楚文件里的内容
	
	//2.设置（初始化）本机的IP和端口、协议
	client_addr.sin_family		= AF_INET; /*协议地址*/  
	client_addr.sin_port		= htons(NET_NUMBER); /*将端口号转换为网络字节序*/ 
	client_addr.sin_addr.s_addr	= inet_addr("224.10.10.1");      /* 绑定的本机IP，0.0.0.0代表本机所有IP htonl(INADDR_ANY) 代表本机所有IP*/
	
	char *ptr = "卢本伟：";
	char arr[100] = {0};
	
	while(1)
	{
		memset(buff , 0, 100);
		memset(arr  , 0, 100);
		printf("输入发送组播的内容:");
		scanf("%s",buff);
		
		sprintf(arr , "%s%s", ptr , buff);
		
		sendto(client_fd, arr, 100, 0,(struct sockaddr *)&client_addr, addr_len);
		
		printf("%s正在广播信息：%s\n",ptr,buff);
	}
	
	
	return 0;
}