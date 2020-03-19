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
#include <errno.h>
#include <pthread.h>

#define MY_NET	   		"192.168.0.108"
#define NET_NUMBER 		50004
#define MAX_LINK_NUMBER 20

int		g_client_fd	=	-1;
int 	client_fds[MAX_LINK_NUMBER]	= {0};
char	rbuff_1[1024]	= {0};
char    arr[64]			= {0};

fd_set	rfdset; // 文件描述符集合

void *tcp_msg_send(void *arg)
{
	while(1)
	{
		bzero(arr,sizeof(arr));
		printf("冲冲冲:\n");
		scanf("%s",arr);
		while(getchar() != '\n');
		send(g_client_fd , arr ,strlen(arr),0 );
		printf("正在给客户端发送消息: %s\n",arr);
		usleep(1000);
	}
}

void *tcp_msg(void *arg)
{
	int  		i;
	long		offset;
	int 		ret				= -1;
	char		array[32]		= {0};
	char 		offsetbuf[1024];
	char 		buffer[1024];
	char		rbuff_1_3[20]	= {0};
	pthread_t 	tid;
	long 		offset_size;
	while(1)
	{
		//查询有无客户端通信
		for(i=0; i<MAX_LINK_NUMBER; i++)
		{
			if(client_fds[i] > 0) // 有客户端
			{
				// 如果该客户端的文件描述符还在集合里，说明有数据可以接收
				if(FD_ISSET(client_fds[i], &rfdset))
				{
					loop:
					memset(rbuff_1, 0, sizeof(rbuff_1));
					ret = recv(client_fds[i], rbuff_1, sizeof(rbuff_1), 0);
					printf("收到客户端消息 %s\n",rbuff_1);
					
					// 当客户端断开时，先关闭它再把其至0
					if(ret <= 0)
					{
						printf("客户端%d断开服务器\n",client_fds[i]);
						close(client_fds[i]);
						client_fds[i] = 0;
						
						break;
					}
					//登录已有账号
					if(strcmp(rbuff_1 , "1") == 0)
					{
						send(client_fds[i], "确认已有账号" , strlen("确认已有账号") ,0 );
						usleep(1000);
						//等待客户端发送具体账号信息和密码
						memset(rbuff_1, 0, sizeof(rbuff_1));
						
						//接收账号
						ret = recv(client_fds[i], rbuff_1, sizeof(rbuff_1), 0);
						printf("账号：%s\n",rbuff_1);
						
						sprintf(array ,"./账号信息/%s.txt" ,rbuff_1);//字符串拼接成为目录名字
						int retval = open(array , O_RDONLY);
						if(retval == -1)
						{
							//printf("open error:");
							send(client_fds[i], "账号错误" , strlen("账号错误") ,0 );
							usleep(1000);
							printf("客户端账号输入错误\n");
							goto loop;
						}
						else
					    {
							send(client_fds[i], "随便啥" , strlen("随便啥") ,0 );
							usleep(1000);
							//接收密码
							memset(rbuff_1, 0, sizeof(rbuff_1));
							ret = recv(client_fds[i], rbuff_1, sizeof(rbuff_1), 0);
							printf("密码：%s\n",rbuff_1);
							read(retval , rbuff_1_3 , sizeof(rbuff_1_3));
							if(strcmp(rbuff_1 , rbuff_1_3) == 0)
							{
								bzero(rbuff_1 ,sizeof(rbuff_1));
								printf("客户端登陆成功!!!\n");
								send(client_fds[i], "随便啥" , strlen("随便啥") ,0 );
								loop_1:
								sleep(2);
								while(1)
								{
									bzero(rbuff_1 ,sizeof(rbuff_1));
									ret = recv(client_fds[i], rbuff_1, sizeof(rbuff_1), 0);
									printf("客户端选择（1：局域网聊天；2：下载文件；其他：共享文件）\n");
									printf("收到客户端消息：%s\n",rbuff_1);
									//客户端聊天
									if(strcmp(rbuff_1 , "1") == 0)
									{
										printf("服务端已确认消息，可以开始聊天了!!!\n");
										send(client_fds[i], "可以聊天了" , strlen("可以聊天了") ,0 );
										pthread_create(&tid , NULL , tcp_msg_send , NULL);
										//聊天
										while(1)
										{
											usleep(1000);
											bzero(rbuff_1 ,sizeof(rbuff_1));
											ret = recv(client_fds[i], rbuff_1, sizeof(rbuff_1), 0);
											if(ret <= 0)
											{
												printf("客户端%d断开服务器!!!\n",client_fds[i]);
												close(client_fds[i]);
												client_fds[i] = 0;
												
												goto loop;
											}
											printf("收到客户端发来的数据：%s\n",rbuff_1);
										}
									}
									//服务端传文件给客户端
									else if(strcmp(rbuff_1 , "2") == 0)
									{
										again:
										usleep(100);
										bzero(rbuff_1 ,sizeof(rbuff_1));
										printf("服务端已确认消息，可以开始传输文件了!!!\n");
										send(client_fds[i], "可以收文件了" , strlen("可以收文件了") ,0 );
										// 首先接收服务器发送的文件名
										usleep(100);
										recv(client_fds[i], rbuff_1, sizeof(rbuff_1), 0);
										
										bzero(array ,sizeof(array));
										sprintf(array ,"./文件目录/%s" ,rbuff_1);
										// 打开文件，如果打不开说明不存在，回复没有这个文件，如果能打开，说明文件存在，回复文件存在
										int file_fd = open(array, O_RDONLY);
										// 判断是否打开成功
										if(file_fd < 0)
										{
											perror("open");

											send(g_client_fd, "fileNoLive", strlen("fileNoLive"), 0);
											goto again;
										}
										printf("文件打开成功\n");
										bzero(rbuff_1,sizeof(rbuff_1));
										
										int file_size = lseek(file_fd,0,SEEK_END);
										sprintf(rbuff_1 , "%d" , file_size);
										// 如果打不开说明不存在，回复没有这个文件，如果能打开，说明文件存在，回复文件存在
										send(g_client_fd, rbuff_1, strlen(rbuff_1), 0); // 回复文件是否存在
										bzero(rbuff_1,sizeof(rbuff_1));
										
										usleep(100);
										// 接收客户端发过来的文件的字节数，根据这个字节数偏移文件指针
										recv(g_client_fd, rbuff_1, sizeof(rbuff_1), 0); // 接收文件字节数
										
										strncpy(offsetbuf, rbuff_1, 32);
										offset = atoi(offsetbuf);
										printf("offset = %ld\n", offset);
										lseek(file_fd, offset, SEEK_SET); // 偏移文件指针
										// 循环读取数据并发送
										while(1)
										{
											usleep(100);
											bzero(rbuff_1,sizeof(rbuff_1));
											ret = read(file_fd, rbuff_1, sizeof(rbuff_1));
											if(ret == 0)
											{
												usleep(10000);
												// 发送结束标志
												//printf("发送了%d字节\n", ret);
												printf("服务端发送数据完成!\n");
												
												close(file_fd);
												goto loop_1;
												// 接收客户端的回复，确保客户端已经接收完成
											}
											else if(ret > 0)
											{
												// 发送文件数据，一次发送1024字节
												usleep(100);
												ret = send(g_client_fd, rbuff_1, ret, 0);
											}
											//打印进度条
											fprintf(stderr , "\r\033[?25l[进度%3.2f%%]] ",(float)lseek(file_fd , 0 ,SEEK_CUR)/file_size * 100);
										}
									}
									//客户端上传文件给服务端
									else
									{
										again_1:
										usleep(100);
										bzero(rbuff_1 ,sizeof(rbuff_1));
										printf("服务端已确认消息，可以开始传输文件了!!!\n");
										send(client_fds[i], "可以发文件了" , strlen("可以收文件了") ,0 );
										// 首先接收客户端发送的文件名
										usleep(100);
										recv(client_fds[i], rbuff_1, sizeof(rbuff_1), 0);
										bzero(array ,sizeof(array));
										sprintf(array ,"./文件目录/%s" ,rbuff_1);
										int file_fd = open(array ,O_RDWR | O_CREAT , 0666);
										usleep(100);
										bzero(rbuff_1 ,sizeof(rbuff_1));
										send(client_fds[i], "文件大小" , strlen("文件大小") ,0 );
										usleep(100);
										//接收到文件大小
										recv(client_fds[i], rbuff_1, sizeof(rbuff_1), 0);
										
										strncpy(offsetbuf, rbuff_1, sizeof(rbuff_1));
										int file_size  = atoi(offsetbuf);
										if(strncmp(rbuff_1, "fileNoLive", sizeof("fileNoLive")) == 0)
										{
											printf("客户端没有该文件!\n");
											goto again_1;
										}
										// 打开文件之后，将文件指针偏移到最后,就是要获取文件当前的大小
										offset_size = lseek(file_fd, 0, SEEK_END); // ftell、stat等等
										printf("offset_size = %ld\n", offset_size);
										bzero(rbuff_1 ,sizeof(rbuff_1));
										// 将当前的文件的字节数发送给客户端
										sprintf(rbuff_1, "%ld", offset_size);
										//把偏移量发送给客户端
										send(client_fds[i], rbuff_1, strlen(rbuff_1), 0); // 有一个函数是将数字的字符串，直接转成数字
										
										// 等待着循环接收数据
										while(1)
										{
											memset(rbuff_1, 0, sizeof(rbuff_1));
											ret = recv(client_fds[i], rbuff_1, sizeof(rbuff_1), 0); // 发送时如果数据缓存区再结构体前面，那么最好用sizeof发完结构体
											
											fprintf(stderr , "\r\033[?25l[进度%3.2f%%]] ",(float)lseek(file_fd , 0 ,SEEK_CUR)/file_size * 100);
											if(ret < 1024)
											{
												write(file_fd, rbuff_1, ret);
												fprintf(stderr , "\r\033[?25l[进度%3.2f%%]] ",(float)lseek(file_fd , 0 ,SEEK_CUR)/file_size * 100);
												printf("客户端发送完成，数据接收完成\n");
												goto loop_1;
											}
											write(file_fd, rbuff_1, ret);
										}
									}
								}
							}
							else
							{
								bzero(rbuff_1 ,sizeof(rbuff_1));
								printf("客户端输入密码错误!\n");
								send(client_fds[i], "密码错误" , strlen("密码错误") ,0 );
								goto loop;
							}
						}
					}
					//注册账号
					else if(strcmp(rbuff_1 , "2") == 0)
					{
						//注册账号
						send(client_fds[i], "注册账号" , strlen("注册账号") ,0 );
						usleep(1000);
						//等待客户端发送具体账号信息和密码
						memset(rbuff_1, 0, sizeof(rbuff_1));
						
						//接收账号
						ret = recv(client_fds[i], rbuff_1, sizeof(rbuff_1), 0);
						if(ret <= 0)
						{
							printf("客户端%d断开服务器\n",client_fds[i]);
							close(client_fds[i]);
							client_fds[i] = 0;
							goto loop;
						}
						printf("账号：%s\n",rbuff_1);
						
						sprintf(array ,"./账号信息/%s.txt" ,rbuff_1);//字符串拼接成为目录名字
						int retval = open(array , O_RDWR | O_CREAT | O_TRUNC , 0777);
						if(retval == -1)
						{
							send(client_fds[i], "创建不成功" , strlen("创建不成功") ,0 );
							usleep(1000);
							printf("客户端账号输入错误\n");
							break;
						}
						else
					    {
							send(client_fds[i], "随便啥" , strlen("随便啥") ,0 );
							usleep(1000);
							//接收密码
							memset(rbuff_1, 0, sizeof(rbuff_1));
							ret = recv(client_fds[i], rbuff_1, sizeof(rbuff_1), 0);
							write(retval , rbuff_1 , strlen(rbuff_1));
							printf("密码：%s\n",rbuff_1);
							printf("新建账号成功!!!\n");
							sleep(2);
						}
					}
					else
					{
						printf("输入无效数字\n");
						send(client_fds[i], "随便啥" , strlen("随便啥") ,0 );
						goto loop;;
					}
				}
			}
		}
	}
}


int	main(void)
{
	int			listen_fd	= -1;
	int			sec_fd_max	= -1;
	pthread_t	tid;
	struct 		sockaddr_in   my_addr; // 绑定本机的IP和端口  
	struct 		sockaddr_in   src_addr; // 存放客户端的IP地址
	socklen_t 	addrlen = sizeof(src_addr);
	
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
	my_addr.sin_port		= htons(NET_NUMBER); /*将端口号转换为网络字节序*/ 
	my_addr.sin_addr.s_addr	= inet_addr(MY_NET);      /* 绑定的本机IP，0.0.0.0代表本机所有IP INADDR_ANY 代表本机所有IP*/
	
	// 设置端口允许重复绑定
	int opt = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
	
	// 3.绑定本机端口和IP地址、协议
	int status = bind (listen_fd, (struct sockaddr *) &my_addr, sizeof(my_addr));
	// 判断绑定是否成功
	if(status < 0)
	{
		perror("band");
		return -1;
	}
	printf("服务端初始化成功，等待客户端连接\n");
	
	// 4. 设置监听 DDOS
	listen(listen_fd, 5); // 同时可以接收5个连接请求
	
	while(1)
	{
		// 清空集合
		FD_ZERO(&rfdset);         // 清空集合
		FD_SET(listen_fd, &rfdset); // 将fd添加到集合里面
		sec_fd_max = listen_fd; // 假设监听套接字是最大值
		
		// 寻找当前需要查询的文件描述符里面的最大值
		for(int i=0; i<MAX_LINK_NUMBER; i++)
		{
			if(client_fds[i] > 0)//大于0就证明这个文件描述符里有数据，可监听
			{
				FD_SET(client_fds[i], &rfdset); // 将fd添加到集合里面
				sec_fd_max = sec_fd_max > client_fds[i] ? sec_fd_max:client_fds[i];// 找出当前客户端里的文件描述符的最大值
			}
		}
		// 使用select多路复用查询是否有客户端连接,且为阻塞函数，没有客户端进入时，会一直阻塞
		select(sec_fd_max + 1, &rfdset, NULL, NULL, NULL);
		
		// 如果有客户端连接，listen_fd里就有数据，我们可以处理（accept）
		if(FD_ISSET(listen_fd, &rfdset)) // 判断listen_fd是否还在集合里面
		{
			//5.开始监听，等待客户端链接 阻塞函数
			memset(&src_addr, 0, sizeof(src_addr));
			g_client_fd = accept(listen_fd, (struct sockaddr *)&src_addr, &addrlen);
			if(g_client_fd < 0)
			{
				perror("accept");
				return -1;
			}
			printf("客户端连接成功\n");
			printf("g_client_fd = %d\n", g_client_fd);
			printf("客户端的地址：%s\n", inet_ntoa(src_addr.sin_addr));
			printf("客户端的端口：%d\n", ntohs(src_addr.sin_port));
			
			for(int i=0; i<MAX_LINK_NUMBER; i++)//这个for循环是为了把新连接的客户端的文件标识符存到集合里
			{
				if(client_fds[i] == 0)
				{
					client_fds[i] = g_client_fd;
					break;
				}
			}
			//创建线程，接收数据
			errno = pthread_create(&tid, NULL, tcp_msg, NULL);
			if(errno != 0)
			{
				perror("creat thread error");
				return -1;
			}
		}
	}
	
	close(listen_fd);
	
	return 0;
}