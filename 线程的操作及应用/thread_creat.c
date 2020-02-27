#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

//这两个函数相当于双线程


void *new_thread(void *arg) //并非函数的调用，而是与main函数同时运行
{
	char *ptr = arg;
	
	while(1)
	{
		sleep(1);
		printf("new thread arg = %s \n",ptr);
	}	
	
	return "world";
	//return NULL; == pthread_exit(NULL);
}

int main(void)
{
	pthread_t tid;
	void *retptr;
	
	//1,id号  2，属性设置,NULL为默认  3,新线程的内容,一个指针函数  4,传到新线程的参数
	errno = pthread_creat(&tid , NULL , new_thread , "hello");
	if(errno != 0)
	{
		perror("creat thread error");
		return -1;
	}
	
	printf("set thread success!\n");
	
	//他会阻塞在这里，一直等到线程的推出
	pthread_join( tid , &retptr );//线程的接合函数
	
	printf("retptr = %s\n",(char *)retptr);
	
	pthread_exit(NULL);  //线程的推出，子线程没退出，所以子线程还是在跑
	
	return 0;
}