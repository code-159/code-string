#include "button.h"

int get_touch_status(int ts_fd, int *x, int *y)
{
	int touch_x, touch_y;
	bool touch_x_flags=false, touch_y_flags=false;
	ssize_t rd_ret;
	struct input_event touch_data;

	while(1)
	{
		rd_ret = read(ts_fd, &touch_data, sizeof(touch_data));	
		if(rd_ret == -1)
		{
			perror("touch device error");
			break;
		}
		else if(rd_ret != sizeof(touch_data))
		{
			continue;
		}
		else
		{
			if(touch_data.type == EV_ABS)
			{
				if(touch_data.code == ABS_X)
				{
					touch_x = touch_data.value*800.0/1024;
					touch_x_flags = true;
				}
				
				if(touch_data.code == ABS_Y)
				{
					touch_y = touch_data.value*480.0/600;
					touch_y_flags = true;
				}

				if(touch_x_flags && touch_y_flags)
				{
					*x = touch_x;
					*y = touch_y;
					goto get_xy_data;
				}
			}
			else if(touch_data.type == EV_KEY)
			{
				if(touch_data.value == 0)
					goto ts_release;
				else
					goto ts_touch;
			}

		}

	}

	return -1;

get_xy_data:
	return TS_GET_XY;

ts_touch:
	return TS_TOUCH;

ts_release:
	return TS_RELEASE;	

}

/*
 
   	goto_activity函数功能：
 			跳转主控界面

	参数：
		int ts_fd;
		int (*init_menu_func)(void)，初始化界面的函数
		int return_menu_func(void)，上层的界面函数
		button arr[]，各类设置按键给他一下
		button_count:用来记录有几个按键
		lcd_fb_addr：显存地址
 	返回值：
		无

 * */


