#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

/*
	定义一个全局变量count初值为10，创建线程1实现互斥锁的初始化，并打印一句init success，
	之后创建3条线程每隔1秒去更改count值，当count值等于0时退出所有线程，
	并退出整个程序（需要用到互斥锁）基于上面的题目，
	实现当程序接收到ctrl+c（SIGINT）这个信号的时候，退出一条线程，并且不能造成死锁
*/

pthread_mutex_t g_mutex;

int count = 10;

void cancel_mutex(void *arg)
{
	pthread_mutex_unlock(&g_mutex);//解锁
	printf("解锁成功!\n");
}

void *new_thread(void *arg)
{
	int odltype;
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS , &odltype);//立即取消
	
	pthread_cleanup_push(cancel_mutex , NULL);
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
	
	pthread_cleanup_pop(1);
	//pthread_mutex_unlock(&g_mutex);//解锁
	
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

void sigquit_handler(int signum)
{
	static int i=0;
	pthread_cancel(&tid[i]);
	i++;
	printf("第%d条线程已经成功退出!\n",i);
	if(i == 3)
	{
		printf("已经没有线程了!\n");
	}
}

int main(void)
{
	signal(SIGINT , sigquit_handler);
	pthread_t tid[4];
	
	pthread_create(&tid[0] , NULL , new_thread_init , NULL);  //创建线程
	
	pthread_join(tid[0], NULL);
	
	for(i=1;i<4;i++)
	{
		pthread_create(&tid[i] , NULL , new_thread , NULL);
	}
	
	pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);
	pthread_join(tid[3], NULL);
	
	
	return 0;
}