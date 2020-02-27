#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>


void *thread(void *arg)
{
	while(1)
	{
		printf("666\n");
		sleep(1);
	}
}


int main(void)
{
	pthread_t tid;
	
	pthread_create(&tid , NULL , thread , NULL);
	
	sleep(5);
	
	pthread_cancel(tid);//取消线程,线程退出信号
	
	pthread_join(tid , NULL);
	
	return 0;
}