#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

pthread_rwlock_t rwlock;

char buffer[1024];

void *read_thread(void *arg)
{
	while(1)
	{
		pthread_rwlock_rdlock(&rwlock);//加上读锁
		
		printf("buffer = %s\n",buffer);
		
		pthread_rwlock_unlock(&rwlock);//解锁
		
		sleep(1);
	}
	
	return NULL;
}

int main(void)
{
	pthread_t tid[2];
	
	pthread_rwlock_init(&rwlock , NULL); //读写锁初始化
	
	pthread_create(&tid[0] , NULL , read_thread , NULL);
	pthread_create(&tid[1] , NULL , read_thread , NULL);
	
	while(1)
	{
		pthread_rwlock_wrlock(&rwlock);//加上写锁
		printf("start input:\n");
		scanf("%s",buffer);
		pthread_rwlock_unlock(&rwlock);//解锁
		sleep(1);
	}
	
	pthread_rwlock_destroy(&rwlock , NULL);//销毁
	
	return 0;
}