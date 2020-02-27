#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>


int main(void)
{   
	char buff[100]={0};
    int sever_fd = socket(AF_INET,SOCK_DGRAM,0);
	if(sever_fd == -1)
	{
		perror("create socket error\n");
		return -1;
	}
    
    struct sockaddr_in src_addr;
	
    socklen_t len = sizeof(src_addr);
    bzero(&src_addr,len);

    src_addr.sin_family = AF_INET;
    src_addr.sin_port = htons(50002); 
	src_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = bind(sever_fd,(struct sockaddr *)&src_addr,len);
	if(ret != 0)
	{
		perror("create bind error\n");
		return -1;
	}

    struct sockaddr_in src_f;
    int src_f_len = sizeof(src_f);
    bzero(&src_f,src_f_len);

    while(1)
    {
        bzero(buff,100);
        recv(sever_fd,buff,100,0);
        printf("来自【%s:%hu】的信息\n",inet_ntoa(src_f.sin_addr),ntohs(src_f.sin_port));
		printf("内容为:%s\n",buff);
    
        sleep(1);
    }

	return 0;
}
