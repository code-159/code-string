#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>

sem_t sem;

void *thread(void *arg)
{
	char *buffer = arg;
	while(1)
	{
		sem_wait(&sem);//P操作
		printf("buffer = %s\n",buffer);
	}
}

int main(void)
{
	char buffer[1024];
	sem_init(&sem , 0 , 0);//无名信号量的初始化
	pthread_t tid;
	
	pthread_create(&tid , NULL , thread , NULL);
	
	while(1)
	{
		scanf("%s",buffer);
		
		sem_post(&sem);   //V操作
	}
	
	pthread_join(&tid , NULL);
	sem_destroy(&sem);//销毁信号量
	
	return 0;
}