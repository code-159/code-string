#include "button.h"


int exit_system(void)//退出系统函数
{
	int lcd_fd8;
	unsigned int *lcd_fd8_addr;

	lcd_fd8 = open_lcd_device(&lcd_fd8_addr);
	
	display_jpeg_format_file("../pic/exit.jpg", 0, 0, lcd_fd8_addr, 1);

	close_lcd_device(lcd_fd8,lcd_fd8_addr);
	
	exit(0);
	
	return 0;
}
