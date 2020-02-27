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
#include <sys/time.h>
#include <signal.h>
#include <assert.h>

int	g_temp_fd1 = -1;
int g_client_fd = -1;

// 如果收到带外数据，进程就会收到SIGURG信号，表示收到了带外数据，所有该函数应该接收带外数据
void  handler(int signo)     
{   
	char rbuffer[100] = {0};
	int ret = recv(g_client_fd, rbuffer, sizeof(rbuffer), MSG_OOB);
	printf("收到 【%d】 字节,rbuffer = %s\n", ret, rbuffer);
	printf("收到了信号:%d\n", signo);
	
	return;  
}

// 任意信号捕捉
void addsig(int sig, void (*handler)(int) )
{	
	struct sigaction  act;
	memset(&act, 0, sizeof(act));
	
	act.sa_handler = handler; // 设置如果收到了SIGALRM信号，就执行handler函数
	act.sa_flags |= SA_RESTART; // 被信号中断的系统调用自动启动
	sigfillset(&act.sa_mask); // 设置全部信号为进程信号的掩码
	assert(sigaction(sig, &act, NULL) != -1); // 信号处理 // 其作用是如果它的条件返回错误，则终止程序执行
	
}

int main(void)
{
	int			listen_fd	= -1; 
	pthread_t	tid			= 0;
	char		rbuff[100]	= {0};
	int 		ret			= -1; 

	
	struct sockaddr_in   my_addr; // 绑定本机的IP和端口  
	struct sockaddr_in   src_addr; // 存放客户端的IP地址
	
	// 1.创建套接字 AF_INET 和 PF_INET 是一样的
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_fd < 0)
	{
		printf("socket create fail!\n");
		return -1;
	}
	
	//2.设置（初始化）本机的IP和端口、协议
	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family		= AF_INET; /*协议地址*/  
	my_addr.sin_port		= htons(50004); /*将端口号转换为网络字节序*/ 
	my_addr.sin_addr.s_addr	= inet_addr("127.0.0.1");      /* 绑定的本机IP，0.0.0.0代表本机所有IP INADDR_ANY 代表本机所有IP*/
	
	// 设置端口允许重复绑定
	int opt = 0;
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
    
	addsig(SIGURG, handler); // 调用函数，将SIGURG信号加入捕获，如果收到了SIGURG信号，就会执行handler
	// 前提条件是进程号必须拿到接收数据的文件描述符
	fcntl(g_client_fd, F_SETOWN, getpid());
		
	while(1)
	{
		ret = recv(g_client_fd, rbuff, sizeof(rbuff), 0);
		printf("recv接收了%d字节\n", ret);
		printf("rbuff = %s\n", rbuff);
		
		if(ret <= 0)
		{
			printf("客户端已退出!\n");
			break;
		}
	}
	

	close(listen_fd);
	//close(g_temp_fd);
	
	return 0;
}