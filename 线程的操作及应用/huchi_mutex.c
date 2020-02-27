#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

pthread_mutex_t mutex;
int glob = 0;
pthread_cond_t cond;//条件变量

void *thread(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);//拿锁
		
		while(glob < 100)
		{	
			printf("tid = %ld sleep \n",pthread_self());  //拿到线程号
		//先解了互斥锁，然后让线程陷入睡眠，当有人过来唤醒的时候再加上互斥锁
			pthread_cond_wait(&cond , &mutex);
			printf("tid = %ld start running\n",pthread_self());
		}
		glob -= 100;
		
		printf("tid = %lu, glob = %d\n",pthread_self() , glob);
		
		pthread_mutex_unlock(&mutex);//解锁
		
		sleep(1);
	}
	return NULL;
	
}

int main (void)
{
	pthread_t tid[2];
	
	pthread_mutex_init(&mutex , NULL);//初始化锁
	pthread_cond_init(&cond , NULL);//条件变量的初始化
	
	pthread_create(&tid[0] , NULL , thread , NULL);
	pthread_create(&tid[1] , NULL , thread , NULL);
	
	sleep(1);
	
	pthread_mutex_lock(&mutex); //加锁
	printf("parent start get glob value\n");
	scanf("%d",&glob);
	pthread_mutex_unlock(&mutex);//解锁

	//pthread_cond_broadcast(&cond);//以广播的方式唤醒所有子线程
	pthread_cond_signal(&cond);//唤醒最先睡眠的子线程
	
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
	
	return 0;
}