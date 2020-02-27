#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
	char buff[100];
	int client_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(client_fd == -1)
	{
		perror("create socket error\n");
		return -1;
	}
	
	struct sockaddr_in src_addr;
	socklen_t src_len = sizeof(src_addr);
	bzero(&src_addr, src_len);

	src_addr.sin_family = AF_INET;
	src_addr.sin_port = htons(50002);
	src_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	while(1)
	{
		bzero(buff, 100);
		scanf("%s",buff);

		sendto(client_fd, buff, 100, 0,(struct sockaddr *)&src_addr, src_len);

		printf("向【%s:%hu】发送信息成功\n",inet_ntoa(src_addr.sin_addr),ntohs(src_addr.sin_port));
		printf("你发送的内容为:%s\n",buff);	
		
		sleep(1);	
	}

	return 0;
}