#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	//1.创建套接字
	int fd = socket(AF_INET, SOCK_DGRAM, 0);			//使用流式套接字，默认TCP协议

	//2.准备跟AF_INET（即IPv4网络层协议）对应的特定IP+PORT地址结构体
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	bzero(&addr, len);

	addr.sin_family = AF_INET;
	//inet_aton("192.168.19.90",&addr.sin_addr);//绑定指定IP，并做字节序的转换
	addr.sin_addr.s_addr = htonl(INADDR_ANY);//自动获取IP,但没有做字节序的转换
	addr.sin_port = htons(50002);

	////4.准备接受对方的地址信息
	struct sockaddr_in peeraddr;
	int peerlen = sizeof(peeraddr);
	bzero(&peeraddr, peerlen);

	//发送信息
	char sendbuf[100];
	bzero(sendbuf, 100);
	
	while(1)
	{
		bzero(sendbuf, 100);
		scanf("%s",sendbuf);
		//strcpy(sendbuf,"rider kick");

		sendto(fd, sendbuf, 100, 0,(struct sockaddr *)&addr, len);

		printf("向【%s:%hu】发送信息：\"%s\"成功\n",
			inet_ntoa(addr.sin_addr),
			ntohs(addr.sin_port),
			sendbuf);
	
		
		

		sleep(1);
			
	}

	return 0;
}