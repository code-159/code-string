#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <poll.h>

#define OPEN_MAX 1024		// 最大打开描述符数

int main(int argc, char const *argv[])
{
	if (2 != argc)
	{
		printf("argement error\n");
		return -1;
	}

	int i;
	int ret;
	int newfd;						// 新连接的客服端描述符
	int maxi;						// 结构体下标最大值
	int nready;						// poll返回值					
	struct pollfd client[OPEN_MAX];	// 最大1024
	char msg[100] = {0};			// 接收客户端消息

	struct sockaddr_in s_addr;		// 服务器地址.端口
	struct sockaddr_in c_addr;		// 客户端地址.端口
	bzero(&s_addr, 0);
	bzero(&c_addr, 0);

	socklen_t len = sizeof(c_addr);	// 客户端地址大小


	// 1.创建socket监听套接字
	int lsnfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == lsnfd)
	{
		perror("socket");
		return -1;
	}

	// 2.设置服务器地址和端口
	s_addr.sin_family 		= AF_INET;
	s_addr.sin_addr.s_addr 	= inet_addr(argv[1]);
	s_addr.sin_port			= htons(50001);

	// 3.允许重复绑定
	int opt = 1;
	setsockopt(lsnfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));

	
	// 4.绑定服务器地址
	int status = bind(lsnfd, (struct sockaddr *)&s_addr, sizeof(s_addr));
	if (-1 == status)
	{
		perror("bind");
		return -1;
	}

	// 5.设置最大监听数
	listen(lsnfd, 128);


	client[0].fd 	 = lsnfd;	// 要监听的第一个文件描述符
	client[0].events = POLLIN;	// lsnfd监听普通读事件

	for (i = 1; i < OPEN_MAX; i++)
		client[i].fd = -1;		// 用-1初始化剩下的元素


	maxi = 0;					// client[]数组有效元素中最大元素下标


	while(1)
	{
		nready = poll(client, maxi+1, -1);	// 阻塞监听是否有客户端连接请求

		if (client[0].revents & POLLIN)	// lsnfd有读事件就绪
		{
			newfd = accept(lsnfd, (struct sockaddr *)&c_addr, &len);
			if (-1 == newfd)
			{
				perror("accept");
				continue;
			}

			printf("[%s:%d]链接上服务器！\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));

			for (i = 0; i < OPEN_MAX; i++)		// 将新连接的放进结构体
			{
				if (client[i].fd < 0)
				{
					client[i].fd = newfd;
					client[i].events = POLLIN;	// 设置刚连接返回的fd,监控读事件

					break;
				}
			}

			if (i == OPEN_MAX)					// 达到最大客户端数
				perror("too many clients");		// 往下执行是由nready读取多少个决定

			if (i > maxi)						// 保存最大下标
				maxi = i;

			if (--nready <= 0)					// 没有更多就绪事件时，继续回到poll阻塞
				continue;

		}


		for (i = 0; i <= maxi; i++)
		{
			if (client[i].fd < 0)			// 此处文件描述符已经关闭
				continue;
			

			if (client[i].revents & POLLIN)	// 读取信息
			{
				if ((ret = recv(client[i].fd, msg, 100, 0)) < 0)
				{
					if (errno == ECONNRESET)	// 收到RST标志
					{
						printf("client[%d] aborted connection\n", i);

						close(client[i].fd);
						client[i].fd = -1;

						continue;
					}
					else
						perror("read error");
				}
				else if (0 == ret)	// 断开连接
				{
					printf("client[%d] closed connection\n", i);
					close(client[i].fd);
					client[i].fd = -1;

					continue;
				}
				else
					printf("服务器:%s\n", msg);					// 打印消息

				if (--nready <= 0)								// 没有更多就绪事件时，继续回到poll阻塞
					break;
			}
		}
	}

	close(lsnfd);

	return 0;
}