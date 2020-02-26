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

void sem_V_opt(int sem_id)
{
	//加操作(v操作)
	
	struct sembuf semb;
	
	semb.sem_num = 0;
	semb.sem_op = 1;
	semb_sem_flg = 0;
	
	semop(sem_id , &semb , 1 );
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
	
	/* union semun init_value;
	init_value.val = 1;
	
	//初始化
	semctl(sem_id , 0 , SETVAL , init_value);
	 */
	 
	char buf[1024];
	while(1)
	{
		scanf("%s",buf);
		
		sem_V_opt(sem_id);//给信号量进行V操作
	}
	
	
	//删除操作
	semctl(sem_id , 1 , IPC_RMID , NULL );
	
	return 0;
}