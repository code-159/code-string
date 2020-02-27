#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

//利用线程的取消机制实现每发送SIGINT信号时退出一条线程

void *new_thread_1(void *arg)
{
	
}

void *new_thread_2(void *arg)
{
	
}

void *new_thread_3(void *arg)
{
	
}

int main(void)
{
	pthread_t tid[3];
	
	errno = pthread_create(&tid[0] , NULL , new_thread_1 , NULL);
	if(errno != 0)
	{
		perror("create tid_1 error");
		return -1;
	}
	
	errno = pthread_create(&tid[1] , NULL , new_thread_2 , NULL);
	if(errno != 0)
	{
		perror("create tid_2 error");
		return -1;
	}
	
	errno = pthread_create(&tid[2] , NULL , new_thread_3 , NULL);
	if(errno != 0)
	{
		perror("create tid_3 error");
		return -1;
	}

	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);
	
	return 0;
}