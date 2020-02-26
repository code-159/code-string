#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/sem.h>

union semun{
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
	(Linux-specific) */
};

void sem_P_opt1(int sem_id)
{
	//减操作（P操作）

	struct sembuf semb;//诞生一个信号量操作结构体数组

	semb.sem_num = 0;//指定操作信号量数组的哪个元素
	semb.sem_op = -1;//信号量的值-1
	semb.sem_flg = 0;//按照标准去操作信号量

	/*
		进行信号量的操作
		sem_id：信号量的ID
		semb：进行何种操作的结构体
		1：操作的元素个数是多少个
	*/
	semop( sem_id, &semb, 1);

}

void sem_P_opt2(int sem_id)
{
	//减操作（P操作）

	struct sembuf semb[1];//诞生一个信号量操作结构体数组

	semb[0].sem_num = 0;//指定操作信号量数组的哪个元素
	semb[0].sem_op = -1;//信号量的值-1
	semb[0].sem_flg = 0;//按照标准去操作信号量

	/*
		进行信号量的操作
		sem_id：信号量的ID
		semb：进行何种操作的结构体
		1：操作的元素个数是多少个
	*/
	semop( sem_id, semb, 1);

}

int main(void)
{
	key_t ipc_key;
	int sem_id;
	
	ipc_key = ftok( "/home/gec", 1);  /*获取IPC对象的key值*/
	if(ipc_key == -1)
	{
		perror("get ipc key error");
		return -1;
	}
	
	//申请一个全局变量(新建一个sem信号量)
	sem_id = semget(ipc_key , 1 , IPC_CREAT|0666);
	if(sem_id == -1)
	{
		perror("get sem error");
		return -1;
	}
	
	union semun init_value;
	init_value.val = 1;
	
	//初始化
	semctl(sem_id , 0 , SETVAL , init_value);
	
	while(1)
	{
		sem_P_opt1(sem_id);
		
		printf("lbwNB\n");
	}
	
	
	//删除操作
	semctl(sem_id , 1 , IPC_RMID , NULL );
	
	return 0;
}