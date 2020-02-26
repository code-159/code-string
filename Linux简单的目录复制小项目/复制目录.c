#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*实现目录拷贝：类似于实现cp命令
	格式：
		./mycpy	目录名	复制成为什么目录
		1，实现目录的拷贝
		2，实现进度的打印		
	建议：
	1，利用利用递归的思想访问目录
	2，记得在新建文件之前要创建好文件夹
	3，一定要记住，你只是打开了目录文件，并没有进去这个目录中，所以你要组合文件名字*/
	
	
//计算文件file_size,返回tell_file_size的值
long get_file_size(FILE *fp)
{
	int file_size,start_offset;
	long tell_file_size,tell_start_offset;
	
	file_size = fseek(fp,0,SEEK_END);         //指到最后
	if(file_size == -1)
	{
        perror("save file size error");
        return -1;
	 }

	tell_file_size = ftell(fp);               //最后到开头的距离大小(字节)
	if(tell_file_size == -1)
    {
		perror("save tell file size error");
        return -1;
    }

	start_offset = fseek(fp,0,SEEK_SET);      //指到开头
	if(start_offset == -1)
	{
		
		perror("save start offfset error");
		return -1;
	}

	return tell_file_size;
}
	

/*     创建符号连接函数，先利用readlink将src_file连接到buffer中
再用symlink将dest_file与buffer连接     */
int copy_link(char *src_file,char *dest_file)
{
	char *buffer=(char *)malloc(sizeof(1024));
	int retval;
	
	retval=readlink(src_file,buffer,sizeof(buffer));
	if(retval=0)
	{
		perror("link error");
		return -1;
	}
	else
	{
		symlink(buffer,dest_file);
	}
	
	free(buffer);
	return 0;
}

	
//实现文件的拷贝，src_file->dest_file 且文件信息和权限都与src_file相同
ssize_t copy_file(const char *src_file,const char *dest_file)
{

	FILE *src_fp,*dest_fp;
	int retval;
	size_t rd_ret,wr_ret;
	char *buffer=(char *)malloc(512*100);
	long file_size,cur_comp_cp_size=0;
	long count=0;

	src_fp = fopen(src_file,"r");
	if(src_fp == NULL)
	{
		perror("open src file error");
		return -1;
	}
	
	dest_fp = fopen(dest_file,"w");
	if(dest_fp == NULL)
	{
		perror("open dest file error");
		return -1;
	}

	
	file_size = get_file_size(src_fp);
	if(file_size == -1 )
	{
		perror("file size error");
		return -1;
	}
	printf("file_size=%ld\n",file_size);
	
	int uone_number=file_size%512;
	
	printf("\n");
	
	while(1)
	{
		rd_ret = fread(buffer,512,100,src_fp);
		if(rd_ret!=100)
		{
			if(ferror(src_fp))
			{
				perror("read src_file error");
				clearerr(src_fp);
				return -1;
			}
			if(feof(src_fp))      //读到末尾  如果是，返回值为真
			{
				fwrite(buffer,512,rd_ret,dest_fp);
				fwrite(buffer+512*rd_ret,uone_number,1,dest_fp);
				printf("\033[1A复制已完成%.6f%%\n",(double)((512*count*100)+(512*rd_ret)+uone_number)/file_size*100);
				break;
			}
			else
			{
				perror("copy error");
				return -1;
			}
		}
		wr_ret = fwrite(buffer,512,100,dest_fp);
		if(wr_ret!=rd_ret)
		{
			if(ferror(dest_fp))
			{
				perror("write dest_file error");
				clearerr(dest_fp);
				return -1;
			}
			
			if(!feof(dest_fp))
			{
				perror("copy error");
				return -1;
			}
		}
		count++;
		cur_comp_cp_size += wr_ret;
		
		printf("\033[1A复制已完成%.6f%%\n", (double)(cur_comp_cp_size*512)/file_size*100);
		
		if(feof(dest_fp))   //读到末尾
			break;
	}

	retval = fclose(dest_fp);
	if(retval == EOF)
	{
		perror("close dest file error");
		return -1;
	}
	
	retval = fclose(src_fp);
	if(retval == EOF)
	{
		perror("close src　file error");
		return -1;
	}
	
	free(buffer);
	return file_size;
}

int my_copy(char *src_dir,char *dest_dir)  //复制目录
{
	int retval;
	DIR *dir1,*dir2;
	struct dirent *file_info;   //目录的信息dirent返回状态
	struct stat info;
	struct stat dir_info;
	char *buffer1=(char *)malloc(1024);
	char *buffer2=(char *)malloc(1024);
	
	lstat(src_dir,&info);   //获取src_dir的信息
	
	dir1=opendir(src_dir);
	if(dir1==NULL)
	{
		perror("open src_dir error");
		return -1;
	}
	
	dir2=opendir(dest_dir);
	if(dir2==NULL)
	{
		mkdir(dest_dir,info.st_mode);
		dir2=opendir(dest_dir);
		if(dir2==NULL)
		{
			perror("open dest_dir error");
			return -1;
		}
	}
	
	while(1)
	{
		bzero(buffer2,sizeof(buffer2));     //清楚buffer2的缓冲区
		file_info=readdir(dir1);
		if(file_info==NULL)     //是否为空
		{
			if(errno!=0)        //是否出错，errno为0为真，不等于0则出错
			{
				return -1;
			}
			else                //读到末尾
			{
				printf("read end of dir\n");
				break;
			}
		}
		if(strcmp(file_info->d_name,".")==0 || strcmp(file_info->d_name,"..")==0)
		{
			continue;
		}
		sprintf(buffer1,"%s/%s",src_dir,file_info->d_name);
		//构造两个完整的相对路径
		sprintf(buffer2,"%s/%s",dest_dir,file_info->d_name);
		
		retval=lstat(buffer1,&dir_info);        //获取buffer1的信息
		if(retval<0)
		{
			perror("failed to get info");
			break;
		}
		
		if(S_ISDIR(dir_info.st_mode)!=0)//判断是否为目录，如果为目录就递归
		{
			my_copy(buffer1,buffer2);
		}
		
		else if(S_ISREG(dir_info.st_mode)!=0)  //普通文件
		{
			copy_file(buffer1,buffer2);
		}
		
		else if(S_ISLNK(dir_info.st_mode)!=0)  //符号链接
		{
			copy_link(buffer1,buffer2);
		}
	}
	
	retval=closedir(dir1);
	if(retval==-1)
	{
		perror("close src_dir error");
		return -1;
	}
	
	retval=closedir(dir2);
	if(retval==-1)
	{
		perror("close dest_dir error");
		return -1;
	}
	printf("复制已经全部完成！！！\n");
	
	free(buffer1);
	free(buffer2);
	return 0;
}

int main (int argc,char *argv[])
{
	
	my_copy(argv[1],argv[2]);
	
	return 0;
}