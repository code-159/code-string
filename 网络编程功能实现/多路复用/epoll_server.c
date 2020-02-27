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
#include <sys/epoll.h>

#define OPEN_MAX 3000		// 最大打开描述符数

int main(int argc, char const *argv[])
{
	if (2 != argc)
	{
		printf("argement error\n");
		return -1;
	}

	int i;
	int ret;
	int efd;						// 指向红黑数的文件描述符
	int newfd;						// 新连接的客服端描述符
	int nready;						// epoll返回值读到个数				

	struct epoll_event tep, ep[OPEN_MAX];


	char msg[100] = {0};			// 接收客户端消息
	struct sockaddr_in s_addr;		// 服务器地址.端口
	struct sockaddr_in c_addr;		// 客户端地址.端口
	bzero(&s_addr, 0);
	bzero(&c_addr, 0);

	socklen_t len = sizeof(c_addr);	// 客户端地址大小


	// 1.创建socket监听套接字
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == listenfd)
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
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));

	
	// 4.绑定服务器地址
	int status = bind(listenfd, (struct sockaddr *)&s_addr, sizeof(s_addr));
	if (-1 == status)
	{
		perror("bind");
		return -1;
	}

	// 5.设置最大监听数
	listen(listenfd, 128);

	// 6.创建epoll模型
	efd = epoll_create(OPEN_MAX);	// 得到指向红黑树的描述符
	if (-1 == efd)
	{
		perror("epoll_create");
		return -1;
	}

	// 将监听对应结构体加入红黑数
	tep.events  = EPOLLIN;		// 读属性
	tep.data.fd = listenfd;
	ret = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &tep);
	if (-1 == ret)
	{
		perror("epoll_ctl");
		return -1;
	}

	while(1)
	{	
		// 阻塞监听是否有客户端连接请求
		nready = epoll_wait(efd, ep, OPEN_MAX, -1);
		if (-1 == nready)
		{
			perror("epoll_wait");
			continue;
		}

		for (i = 0; i < nready; i++)
		{
			if (!(ep[i].events & EPOLLIN))	// 如果不是读事件，继续循环
				continue;

			if (ep[i].data.fd == listenfd)	// 表示有客户端链接
			{
				newfd = accept(listenfd, (struct sockaddr *)&c_addr, &len);
				if (-1 == newfd)
				{
					perror("accept");
					continue;
				}

				printf("[%s:%d]链接上服务器！\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));

				tep.events  = EPOLLIN;
				tep.data.fd = newfd;
				ret = epoll_ctl(efd, EPOLL_CTL_ADD, newfd, &tep); 
				if (-1 == ret)
					perror("epoll_ctl");
			}
			else	// 游客户端发数据过来
			{
				int sockfd = ep[i].data.fd;
				if ((ret = recv(sockfd, msg, 100, 0)) < 0)
				{
					if (errno == ECONNRESET)	// 收到RST标志
					{
						printf("client aborted connection\n");

						ret = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);
						if (-1 == ret)
							perror("epoll_ctl");

						close(sockfd);

						continue;
					}
				}
				else if (0 == ret)	// 断开连接
				{
					printf("client closed connection\n");

					ret = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);
					if (-1 == ret)
						perror("epoll_ctl");

					close(sockfd);

					continue;
				}
				else
					printf("服务器:%s\n", msg);					// 打印消息
			}
		}

	}

	close(listenfd);
	close(efd);

	return 0;
}