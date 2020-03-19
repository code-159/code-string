#ifndef __BUTTON_H_
#define __BUTTON_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <linux/input.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <errno.h>

#define	TS_TOUCH	0
#define	TS_RELEASE	1
#define	TS_GET_XY	2

typedef struct {
	int start_x;
	int start_y;
	int length;
	int height;
	char touch_pic[1024];
	char release_pic[1024];
}button;

int first_lcd(void);
int first_lcd_return(void);
int exit_system(void);
int play_list(void);
int play_list_2(void);
int play_1(void);
int play_2(void);
int play_3(void);
int play_4(void);
int play_5(void);
int play_6(void);
int write_type(unsigned int x,unsigned int y,char buf[]);

void goto_activity(int ts_fd, int (*init_menu_func)(unsigned int *, button bt_arr[], int), button bt_arr[], int button_count, int (*return_menu_func)(unsigned int *, button *, int), unsigned int *lcd_fb_addr);

int open_touchscreen_device(void);
int close_touchscreen_device(int ts_fd);
int main_menu(unsigned int *lcd_fb_addr, button bt_arr[], int button_count);
int open_lcd_device(unsigned int **lcd_fb_addr);
int close_lcd_device(int lcd_fd, unsigned int *lcd_fb_addr);
int display_jpeg_format_file(const char *filename, unsigned int x_s, unsigned int y_s, unsigned int *lcd_buf_ptr, float mul);
int clear_lcd();
int get_touch_status(int ts_fd, int *x, int *y);


#endif
