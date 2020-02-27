#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <pthread.h>

//这两个函数相当于双线程


void *new_thread(void *arg) //并非函数的调用，而是与main函数同时运行
{
	char *ptr = arg;
	size_t stack_size;
	pthread_attr_t attr;
	
	//获取线程的属性结构体的设置
	errno = pthread_getattr_np(pthread_self() , &attr);
	if(errno != 0)
	{
		perror("get thread attr error");
		return NULL;
	}
	
	//获取线程的栈大小
	errno = pthread_attr_getstacksize(&attr , &stack_size);
	if(errno != 0)
	{
		perror("get thread stack size error");
		return NULL;
	}
	
	printf("thread stack size is %ld\n",stack_size);
	
	pthread_attr_destroy(&attr);//系统要求：如果不是长时间需要用到线程属性这个结构体，在设置或者是获得线程属性之后，应尽快销毁线程属性
	
	return "world";
	//return NULL; == pthread_exit(NULL);
}

int main(void)
{
	pthread_t tid;
	void *retptr;
	pthread_attr_t attr; //创建一个线程属性的结构体
	
	pthread_attr_init(&attr);//线程属性的初始化
	
	//1,id号  2，属性设置,NULL为默认  3,新线程的内容,一个指针函数  4,传到新线程的参数
	errno = pthread_create(&tid , &attr , new_thread , NULL);
	if(errno != 0)
	{
		perror("creat thread error");
		return -1;
	}
	
	printf("set thread success!\n");
	
	//他会阻塞在这里，一直等到指定线程的退出
	errno = pthread_join( tid , &retptr );//线程的接合函数
	if(errno != 0)
	{
		perror("join thread error");
		return -1;
	}
	
	
	printf("retptr = %s\n",(char *)retptr);
	
	pthread_attr_destroy(&attr);  //线程的销毁
	
	pthread_exit(NULL);  //线程的推出，子线程没退出，所以子线程还是在跑
	
	return 0;
}