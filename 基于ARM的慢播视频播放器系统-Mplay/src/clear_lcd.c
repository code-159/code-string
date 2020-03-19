#include "button.h"

int open_lcd_device(unsigned int **lcd_fb_addr)
{
	int lcd_fd;
	
	lcd_fd = open("/dev/fb0", O_RDWR);
	if(lcd_fd == -1)
	{
		perror("open lcd device failed\n");
		return -1;
	}

	*lcd_fb_addr = mmap(NULL, 800*480*4, PROT_READ|PROT_WRITE, MAP_SHARED, lcd_fd, 0);
	if(*lcd_fb_addr == MAP_FAILED)
	{
		perror("map lcd freambuffer error\n");
		return -1;
	}

	return lcd_fd;
}

int close_lcd_device(int lcd_fd, unsigned int *lcd_fb_addr)
{
	
	munmap(lcd_fb_addr, 800*480*4);

	return close(lcd_fd);
}

void lcd_display_point(int x, int y, unsigned int color, unsigned int *lcd_fb_addr)
{
	*(lcd_fb_addr+800*y+x) = color;
}
//实现清屏功能，让屏幕全显示为黑色
int clear_lcd()
{
	int *ptr=NULL;
	int i,j,fd,retval;
	int lcd_fd;
	unsigned int *lcd_fd_addr;

	lcd_fd = open_lcd_device(&lcd_fd_addr);
	
	fd=open("/dev/fb0",O_RDWR);
	if(fd == -1)
	{
		perror("open file error");
		return -1;
	}
	//映射内存
	ptr=mmap(NULL,480*800*4,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
	if(ptr == MAP_FAILED)
	{
		perror("map lcd freambuffer error\n");
		return -1;
	}
	for(i=0;i<480;i++)
	{
		for(j=0;j<800;j++)
		{
			*(ptr+800*i+j)=0x000000;
		}
	}	

	return 0;
}
