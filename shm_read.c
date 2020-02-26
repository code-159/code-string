#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/shm.h>

int main(void)
{
	key_t ipc_key;
	int shm_id , retval;
	char *ptr;
	
	/*获取IPC对象的key值*/
	ipc_key = ftok( "/home/gec", 1);  /*获取IPC对象的key值*/
	if(ipc_key == -1)
	{
		perror("get ipc key error");
		return -1;
	}
	
	shm_id = shmget( ipc_key , 4096 , IPC_CREAT|0666 ); //创建好共享内存,并拿到ID
	if(shm_id == -1)
	{
		perror("get share memory ID error");
		return -1;
	}
	
	//将内核层的共享内存映射到该进程的虚拟内存上
	ptr = shmat( shm_id , NULL , 0); //内存映射,shmat函数
	if(ptr == (void *)-1) //强制转换
	{
		perror("map shm error");
		return -1;
	}
	
	printf("%s",ptr);
	
	
	retval = shmdt(ptr); //取消映射
	if(retval == -1)
	{
		perror("unmap shm error");
		return -1;
	}
	
	//shmctl(shm_id , IPC_RMID , NULL);//删除共享内存的ipc对象
	
	return 0;
}