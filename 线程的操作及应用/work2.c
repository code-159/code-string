#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
/*
	定义一全局的buffer字符串，开启两条线程，一条专门用于每隔1秒打印buffer变量中的值，
	一条用于监控键盘，当键盘输入数据就去改变buffer变量，输入exit则退出整个进程
*/

char buffer[1024] = {0};

void *new_thread_1(void *arg)
{
	while(1)
	{
		scanf("%s",buffer);
		if(strcmp(buffer , "exit") == 0)
			exit(0);
	}
}

void *new_thread_2(void *arg)
{
	while(1)
	{
		printf("buffer = %s\n",(char *)arg);
		sleep(1);
	}
}

int main(void)
{
	pthread_t tid[2];
	
	errno = pthread_create(&tid[0] , NULL , new_thread_1 , buffer);
	if(errno != 0)
	{
		perror("create tid_1 error");
		return -1;
	}
	
	errno = pthread_create(&tid[1] , NULL , new_thread_2 , buffer);
	if(errno != 0)
	{
		perror("create tid_2 error");
		return -1;
	}
	

	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	
	return 0;
}