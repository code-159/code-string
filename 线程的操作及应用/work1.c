#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

/*
	定义一个全局变量count，创建线程1实现互斥锁的初始化，并打印一句init success
	，之后创建3条线程去更改count值，当count值等于0时退出所有线程，并退出整个程序
	exit(0);
*/

pthread_mutex_t g_mutex;

int count = 13;

void *new_thread(void *arg)
{
	//pthread_mutex_init(&g_mutex , NULL);  //初始化锁
	while(count != 0)
	{
		pthread_mutex_lock(&g_mutex);  //拿锁
		if(count>0)
		{
			printf("count = %d\n",count);
			sleep(1);
			count--;
		}
		pthread_mutex_unlock(&g_mutex);//解锁
	}
	
	printf("count = %d\n",count);

	exit(0);
	
	return NULL;
	
	//pthread_mutex_unlock(&g_mutex);//解锁
	pthread_mutex_destroy(&g_mutex);//销毁锁变量资源
}


void *new_thread_init(void *arg)
{
	pthread_mutex_init(&g_mutex , NULL);  //初始化锁
	
	printf("init succese\n");
	
}

int main(void)
{
	pthread_t tid[4];
	
	errno = pthread_create(&tid[0] , NULL , new_thread_init , NULL);  //创建线程
	if(errno != 0)
	{
		perror("create tid_0 error");
		return -1;
	}
	
	pthread_join(tid[0], NULL);
	
	errno = pthread_create(&tid[1] , NULL , new_thread , NULL);  //创建线程
	if(errno != 0)
	{
		perror("create tid_1 error");
		return -1;
	}
	
	errno = pthread_create(&tid[2] , NULL , new_thread , NULL);  //创建线程
	if(errno != 0)
	{
		perror("create tid_2 error");
		return -1;
	}
	
	errno = pthread_create(&tid[3] , NULL , new_thread , NULL);  //创建线程
	if(errno != 0)
	{
		perror("create tid_3 error");
		return -1;
	}
	
	pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);
	pthread_join(tid[3], NULL);
	
	
	
	return 0;
}