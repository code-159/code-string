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
#include <pthread.h>

#define MY_NET	    "192.168.0.108"
#define NET_NUMBER 	50004

int	client_fd	= -1;
char rbuff_1[64] = {0};

void *tcp_msg_recv(void *arg)
{
	int ret = -1;
	while(1)
	{
		usleep(1000);
		bzero(rbuff_1 ,sizeof(rbuff_1));
		ret = recv(client_fd , rbuff_1, sizeof(rbuff_1), 0);
		if(ret <= 0)
		{
			printf("服务端已退出!!!\n");
			exit(0);
		}
		printf("收到服务端发来的数据：%s\n",rbuff_1);
	}
}

int main(void)
{
	char		buffer[1024]	= {0};
	char   		arr[1024]		= {0};
	char		arr_number[32]	= {0};
	char 		array[1024]		= {0};
	char 		offsetbuf[1024];
	long		file_size;
	long 		offset_size;
	long		offset;
	int 		file_fd;
	pthread_t 	tid;
	struct  	sockaddr_in   server_addr; // 绑定本机的IP和端口  
	
	// 1.创建套接字 AF_INET 和 PF_INET 是一样的
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd < 0)
	{
		printf("socket create fail!\n");
		return -1;
	}
	
	//2.设置（初始化）本机的IP和端口、协议
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family		= AF_INET; /*协议地址*/  
	server_addr.sin_port		= htons(NET_NUMBER); /*将端口号转换为网络字节序*/ 
	server_addr.sin_addr.s_addr	= inet_addr(MY_NET);
	
	socklen_t addrlen = sizeof(server_addr);
	
	int ret = connect(client_fd, (struct sockaddr *)&server_addr, addrlen);
	if(ret < 0)
	{
		perror("connect\n");
		return -1;
	}

	printf("成功连接服务器\n");
	usleep(10000);
	
	loop:
	memset(arr , 0 , sizeof(arr));
	printf("\n请输入你的选项（1：登录已有账号；2：申请注册账号）: \n");
	scanf("%s",arr);
	send(client_fd , arr ,strlen(arr),0 );
	usleep(1000);
	printf("arr = %s\n",arr);
	
	recv(client_fd , arr, sizeof(arr), 0);//等待客户端确认信息
	if(strcmp(arr , "确认已有账号") == 0)
	{
		//输入账号
		memset(arr_number , 0 , sizeof(arr_number));
		memset(arr , 0 , sizeof(arr));
		printf("输入账号：\n");
		while(getchar() != '\n');
		scanf("%s",arr_number);
		//发送账号给客户端确认
		send(client_fd , arr_number ,strlen(arr_number),0 );
		printf("账号：%s\n",arr_number);
		
		usleep(1000);
		recv(client_fd , arr, sizeof(arr), 0);//等待客户端确认信息
		//printf("arr = %s\n",arr);
		if(strcmp(arr ,"账号错误") == 0)//打开不成功，重新输入，进入第50行选择
		{
			printf("账号错误\n");
			goto loop;
		}
		//输入密码
		printf("输入密码：\n");
		bzero(arr,sizeof(arr));
		scanf("%s",arr);
		//发送密码给客户端确认
		send(client_fd , arr ,strlen(arr),0 );
		printf("密码：%s\n",arr);
		
		bzero(arr,sizeof(arr));
		usleep(1000);
		recv(client_fd , arr, sizeof(arr), 0);//等待客户端确认信息
		if(strcmp(arr ,"随便啥") == 0)//打开不成功，重新输入，进入第50行选择
		{
			printf("账号登陆成功\n");
			loop_1:
			sleep(2);
			while(1)
			{
				printf("输入信息（1：局域网聊天；2：下载文件；其他：共享文件）\n");
				bzero(arr,sizeof(arr));
				scanf("%s",arr);
				send(client_fd , arr ,strlen(arr),0 );
				printf("正在给服务端发送:%s\n",arr);
				usleep(1000);
				recv(client_fd , arr, sizeof(arr), 0);//等待客户端确认信息
				//客户端与服务端聊天
				if(strcmp(arr , "可以聊天了") == 0)
				{
					pthread_create(&tid , NULL , tcp_msg_recv , NULL);
					while(1)
					{
						printf("输入信息:\n");
						bzero(arr,sizeof(arr));
						scanf("%s",arr);
						while(getchar() != '\n');
						send(client_fd , arr ,strlen(arr),0 );
						printf("正在给服务端发送消息: %s\n",arr);
						usleep(1000);
					}
				}
				//服务端传输文件给服务端
				else if(strcmp(arr , "可以收文件了") == 0)
				{
					again:
					bzero(arr ,sizeof(arr));
					bzero(buffer,sizeof(buffer));
					// 客户端发送文件名字给服务器
					printf("请输入要下载的文件名:");
					scanf("%s", arr);
					send(client_fd, arr, strlen(arr), 0);
					sprintf(array ,"./账号信息/%s/%s" ,arr_number , arr);
					usleep(100);
					// 接收服务器回复的消息
					recv(client_fd, buffer, sizeof(buffer), 0);
					if(strncmp(buffer, "fileNoLive", sizeof("fileNoLive")) == 0)
					{
						printf("客户端没有该文件!\n");
						goto again;
					}
					
					else
					{
						file_fd = open(array, O_RDWR | O_CREAT , 0666);
						if(file_fd < 0)
						{
							perror("file_fd");
							return -1;
						}
						
						strncpy(offsetbuf, buffer, 32);
						file_size  = atoi(offsetbuf);
						printf("文件打开/创建成功\n");
					}
					
					// 打开文件之后，将文件指针偏移到最后,就是要获取文件当前的大小
					offset_size = lseek(file_fd, 0, SEEK_END); // ftell、stat等等
					printf("offset_size = %ld\n", offset_size);
					// 将当前的文件的字节数发送给服务端
					sprintf(arr, "%ld", offset_size);
					send(client_fd, arr, strlen(arr), 0); // 有一个函数是将数字的字符串，直接转成数字
					
					// 等待着循环接收数据
					while(1)
					{
						memset(arr, 0, sizeof(arr));
						ret = recv(client_fd, arr, sizeof(arr), 0); // 发送时如果数据缓存区再结构体前面，那么最好用sizeof发完结构体
						
						fprintf(stderr , "\r\033[?25l[进度%3.2f%%]] ",(float)lseek(file_fd , 0 ,SEEK_CUR)/file_size * 100);
						if(ret < 1024)
						{
							write(file_fd, arr, ret);
							fprintf(stderr , "\r\033[?25l[进度%3.2f%%]] ",(float)lseek(file_fd , 0 ,SEEK_CUR)/file_size * 100);
							printf("服务器发送完成，数据接收完成\n");
							goto loop_1;
						}
						write(file_fd, arr, ret);
					}
				}
				//客户端共享文件给服务端
				else if(strcmp(arr , "可以发文件了") == 0)
				{
					again_1:
					bzero(arr ,sizeof(arr));
					bzero(buffer,sizeof(buffer));
					// 客户端发送文件名字给服务器
					printf("请输入要下载的文件名:");
					scanf("%s", arr);
					sprintf(array ,"./账号信息/%s/%s" ,arr_number , arr);
					send(client_fd, arr, strlen(arr), 0);
					usleep(100);
					bzero(arr ,sizeof(arr));
					recv(client_fd, arr, sizeof(arr), 0);
					int file_fd = open(array, O_RDONLY);
					// 判断是否打开成功
					if(file_fd < 0)
					{
						perror("open");
						send(client_fd, "fileNoLive", strlen("fileNoLive"), 0);
						goto again_1;
					}
					printf("文件打开成功\n");				
					int file_size = lseek(file_fd,0,SEEK_END);
					sprintf(buffer , "%d" , file_size);
					//发送文件大小
					bzero(arr,sizeof(arr));	
					send(client_fd, buffer, strlen(buffer), 0);
					
					bzero(arr ,sizeof(arr));
					recv(client_fd, arr, sizeof(arr), 0); // 接收文件字节数
					strncpy(offsetbuf, arr, sizeof(arr));
					offset = atoi(offsetbuf);
					printf("offset = %ld\n", offset);
					lseek(file_fd, offset, SEEK_SET); // 偏移文件指针
					
					while(1)
					{
						usleep(100);
						bzero(arr,sizeof(arr));
						// 读取文件内容，一次读取1024字节
						ret = read(file_fd, arr, sizeof(arr));
						if(ret == 0)
						{
							usleep(1000);
							//printf("发送了%d字节\n", ret);
							printf("服务端发送数据完成!\n");			
							close(file_fd);
							goto loop_1;
							// 接收客户端的回复，确保客户端已经接收完成
						}
						if(ret <= 0)
						{
							printf("客户端断开连接!!!\n");
							close(file_fd);
							exit(0);
						}
						else if(ret > 0)
						{
							// 发送文件数据，一次发送1024字节
							usleep(100);
							ret = send(client_fd, arr, ret, 0);
						}
						//打印进度条
						fprintf(stderr ,"\r\033[?25l[进度%3.2f%%]] ",(float)lseek(file_fd , 0 ,SEEK_CUR)/file_size * 100);
					}
				}
				
				else
					printf("输入有误，请重新输入!!!\n");
				goto loop_1;
			}
		}
		else
		{
			printf("输入密码错误!\n");
			sleep(2);
			goto loop;
		}
	}
	else if(strcmp(arr , "注册账号") == 0)
	{
		//输入账号
		memset(arr , 0 , sizeof(arr));
		memset(arr_number , 0 , sizeof(arr_number));
		printf("输入账号：\n");
		while(getchar() != '\n');
		scanf("%s",arr);
		
		sprintf(arr_number,"./账号信息/%s" , arr);
		mkdir(arr_number , S_IRWXU);  //在客户端账号信息内创建目录
		send(client_fd , arr ,strlen(arr),0 );
		printf("账号：%s\n",arr);
		
		usleep(1000);
		memset(arr , 0 , sizeof(arr));
		recv(client_fd , arr, sizeof(arr), 0);//等待客户端确认信息
		//printf("arr = %s\n",arr);
		if(strcmp(arr ,"创建不成功") == 0)//打开不成功，重新输入，进入第50行选择
		{
			printf("%s\n",arr);
			goto loop;
		}
		//输入密码
		printf("输入密码：\n");
		//while(getchar() != '\n');
		bzero(arr,sizeof(arr));
		scanf("%s",arr);

		send(client_fd , arr ,strlen(arr),0 );
		printf("密码：%s\n",arr);
		printf("新建账号成功!!!\n");
		sleep(2);
		goto loop;
	}
	
	else
		goto loop;
	
	over:
	return 0;
}