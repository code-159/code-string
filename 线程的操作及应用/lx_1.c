#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/*
	编写一个程序，并且新建5条线程，当主线程从键盘接收到exit指定的
	，所有的立即线程退出给主线程接合
*/

void *thread(void *arg)
{
	while(1)
	{
		printf("fuck you\n");
		sleep(5);
	}
	
	return NULL;
}

int main(void)
{
	pthread_t tid[5];
	int i;
	char buffer[1024]={0};
	
	for(i=0;i<5;i++)
	{
		pthread_create(&tid[i] , NULL , thread , NULL);
	}
	
	while(1)
	{
		scanf("%s",buffer);
		if(strcmp( buffer , "exit") == 0)
		{
			for(i=0;i<5;i++)
			{
				pthread_cancel(tid[i]);
				//printf("666\n");
			}
			//exit(0);
		}
	}
	
	
	for(i=0;i<5;i++)
	{
		pthread_join(tid[i] , NULL);
	}
	
	
	return 0;
}