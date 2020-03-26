#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>
#include <stdlib.h>

#define DATABASE "stu.db"

//插入数据到数据库中
int do_insert(sqlite3* db)
{
	int id;
	char name[32]={0};
	int score;
	char sql[128]={0};
	char *errmsg;
	
	printf("Input id:");
	scanf("%d",&id);
	getchar();
	
	printf("Input name:");
	scanf("%s",name);
	getchar();
	
	printf("Input score:");
	scanf("%d",&score);
	getchar();
	
	sprintf(sql,"insert into stu values(%d,'%s',%d);",id,name,score);
	
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
	{
		printf("%s\n",errmsg);
	}
	else
	{
		printf("Insert success!\n\n");
	}
	
	return 0;
}

//以id方式来删除
int do_delete(sqlite3* db)
{
	int id;
	char sql[128]={0};
	char *errmsg;
	
	printf("Input id:");
	scanf("%d",&id);
	getchar();
	
	sprintf(sql,"delete from stu where id = %d;",id);
	
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
	{
		printf("%s\n",errmsg);
	}
	else
	{
		printf("Delete success!\n\n");
	}
	
	return 0;
}

//查询时回调函数
int callback(void *para,int f_num,char **f_value,char **f_name)
{
	int i = 0;
	
	for(i = 0; i < f_num ; i ++)
	{
		printf("%-11s ",f_value[i]);
	}
	putchar(10);
	
	return 0;
}

//查询函数1
int do_query_1(sqlite3* db)
{
	char sql[128]={};
	char *errmsg;
	
	sprintf(sql,"select * from stu;");
	
	if(sqlite3_exec(db,sql,callback,NULL,&errmsg)!=SQLITE_OK)
	{
		printf("%s\n",errmsg);
	}
	else
	{
		printf("Query success!\n\n");
	}
	
}

//查询函数2
int do_query_2(sqlite3* db)
{
	char sql[128]={0};
	char *errmsg;
	char **resultp;
	int nrow;
	int ncloumn;
	int index;
	
	sprintf(sql,"select * from stu;");
	
	if(sqlite3_get_table(db,sql,&resultp,&nrow,&ncloumn,&errmsg)!=SQLITE_OK)
	{
		printf("%s\n",errmsg);
	}
	else
	{
		printf("Query success!\n\n");
	}
	
	//打印台头
	for(int j = 0; j < ncloumn ; j++)
	{
		printf("%-11s ",resultp[j]);
	}
	putchar(10);
	
	index = ncloumn;
	for(int i = 0; i<nrow ; i++)
	{
		for(int j = 0; j <ncloumn ; j++)
		{
			printf("%-11s ",resultp[index++]);
		}
		putchar(10);
	}
	
	return 0;
}

//以id号来修改(更新)score
int do_update(sqlite3* db)
{
	int id;
	int score;
	char sql[128]={0};
	char *errmsg;
	
	printf("Input id:");
	scanf("%d",&id);
	getchar();
	
	printf("Input score:");
	scanf("%d",&score);
	getchar();
	
	sprintf(sql,"update stu set score = %d where id = %d;",score,id);
	
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
	{
		printf("%s\n",errmsg);
	}
	else
	{
		printf("Update success!\n\n");
	}
	
	return 0;
}



int main(int argc ,const char *argv[])
{
	sqlite3* db;//句柄
	char* errmsg;//出错的情况
	int input_cmd;
	
	if(sqlite3_open(DATABASE ,&db) != SQLITE_OK)
	{
		printf("%s,\n",sqlite3_errmsg(db));
		return -1;
	}
	else
	{
		printf("Open DATABASE success!\n");
	}
	
	//创建一张数据库的表格
	if(sqlite3_exec(db,"create table stu(id Integer ,name char ,score Integer);",NULL,NULL,&errmsg)!=SQLITE_OK)
	{
		printf("%s\n",errmsg);
	}
	else//成功
	{
		printf("create table or open success!\n");
	}
	
	while(1)
	{
		sleep(1);
		printf("\n******************************************\n");
		printf("1:insert 2:delete 3:query 4:update 5:quit.\n");
		printf("******************************************\n");
		
		printf("Input cmd:");
		scanf("%d",&input_cmd);
		while(getchar()!='\n');
		
		switch(input_cmd)
		{
			case 1:
				do_insert(db);
				break;
			case 2:
				do_delete(db);
				break;
			case 3:
				//do_query_1(db);
				do_query_2(db);
				break;
			case 4:
				do_update(db);
				break;
			case 5:
				sqlite3_close(db);
				exit(0);
			default:
				printf("Error input_cmd.\n");
		}
	}
	
	return 0;
}