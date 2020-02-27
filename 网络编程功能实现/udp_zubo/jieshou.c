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
	int  server_fd 	= -1;
	int  retval		= -1;
	int	 set		=  1;
	char buff[100]  = {0};
	
	struct sockaddr_in server_addr; // 绑定本机的IP和端口  
	struct sockaddr_in recv_addr;
	socklen_t addr_len = sizeof(server_addr);
	socklen_t recv_len = sizeof(recv_addr);
	
	// 1.创建套接字 AF_INET 和 PF_INET 是一样的
	server_fd = socket(AF_INET, SOCK_DGRAM, 0);  //以UDP的方式来通信
	if(server_fd == -1)
	{
		perror("socket create fail!\n");
		return -1;
	}
	
/* struct ip_mreq
{
	struct  in_addr  imr_multiaddr;
	struct  in_addr  imr_interface;
}; */
	//设置多播组
	struct ip_mreq  mreq;
	bzero(&mreq, sizeof(mreq));
	mreq.imr_multiaddr.s_addr = inet_addr("224.10.10.1");
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET , MY_NET, (void *)& mreq.imr_interface);
	
	setsockopt(server_fd, SOL_SOCKET , SO_REUSEADDR, &set, sizeof(set));
	setsockopt(server_fd ,IPPROTO_IP , IP_ADD_MEMBERSHIP , &mreq , sizeof(mreq));
	printf("成功加入多播组!\n");
	
	memset(&server_addr, 0, sizeof(server_addr));//清楚文件里的内容
	
	//2.设置（初始化）本机的IP和端口、协议
	server_addr.sin_family		= AF_INET; /*协议地址*/  
	server_addr.sin_port		= htons(NET_NUMBER); /*将端口号转换为网络字节序*/ 
	server_addr.sin_addr.s_addr	= INADDR_ANY;      /* 绑定的本机IP，0.0.0.0代表本机所有IP htonl(INADDR_ANY) 代表本机所有IP*/
	
	// 3.绑定本机端口和IP地址、协议
	retval = bind (server_fd, (struct sockaddr *) &server_addr, addr_len);
	if(retval == -1)
	{
		perror("bind error\n");
		return -1;
	}
	printf("绑定成功!\n");
	
	while(1)
	{
		memset(buff, 0 ,100);
		memset(&recv_addr, 0 , recv_len);
		
		recvfrom(server_fd, buff, 100, 0 ,(struct sockaddr *)&recv_addr, &recv_len);
		
		printf("接收到地址为%s,端口号为%d 的信息：%s\n",inet_ntoa(recv_addr.sin_addr),ntohs(recv_addr.sin_port),buff);
	}
	
	return 0;
}