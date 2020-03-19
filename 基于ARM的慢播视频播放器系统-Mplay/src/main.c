#include "button.h"

/*
	项目思路：mian函数进入，显示开机界面，睡眠三秒后，进入播放列表的函数
	播放列表函数包括两页，两个函数，第一个列表中有七个按钮，四个播放视频函数跳转
	三个按钮（上下页和退出系统按钮），进入视频函数后，视频函数也有七个按钮，分别
	对应七个不同的操作；第二个列表函数中两个播放视频函数，播放视频函数对应一个线程
	当点击相应按钮操作就是操作线程，当退出或者进入播放列表函数时，先杀死线程在后台
	播放的视频，再进行函数的跳转，这就是这个项目的整个框架。
*/

int main(void)
{
	clear_lcd();//清屏

	int first_lcd,button_fd;

	unsigned int *lcd_fb_addr;

	first_lcd = open_lcd_device(&lcd_fb_addr);

	display_jpeg_format_file("../pic/开机.jpg", 0, 0, lcd_fb_addr, 1);
	
	sleep(3);
	
	play_list();     //直接调用视屏列表函数

	close_lcd_device(first_lcd,lcd_fb_addr);
	
	return 0;
}
