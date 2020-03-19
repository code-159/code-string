
#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>
#include "button.h"

extern JSAMPLE * image_buffer;	/* Points to large array of R,G,B-order data */
extern int image_height;	/* Number of rows in image */
extern int image_width;		/* Number of columns in image */

extern void lcd_display_point(int x, int y, unsigned int color, unsigned int *lcd_fb_addr);

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


/*
 * Sample routine for JPEG decompression.  We assume that the source file name
 * is passed in.  We want to return 1 on success, 0 on error.
 */


int display_jpeg_format_file(const char *filename, unsigned int x_s, unsigned int y_s, unsigned int *lcd_buf_ptr, float mul)
{
  
	struct jpeg_decompress_struct cinfo;
  
	struct my_error_mgr jerr;
	FILE * infile;		/* source file */
	int row_stride;		/* physical row width in output buffer */
 
	if((infile = fopen(filename, "rb")) == NULL) 
	{
		fprintf(stderr, "can't open %s\n", filename);
		return 0;
	}

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
  
	if(setjmp(jerr.setjmp_buffer)) 
	{    
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return 0;
	}

	jpeg_create_decompress(&cinfo);

	jpeg_stdio_src(&cinfo, infile);
	
	jpeg_read_header(&cinfo, TRUE);
    
	jpeg_start_decompress(&cinfo);
  
	row_stride = cinfo.output_width * cinfo.output_components;

	char *buffer;		/* Output row buffer */
  
	buffer =(char *)(*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	unsigned int x, y = y_s;
	unsigned int color;
	char *buf_save = buffer;
	float point_distance = 1/mul;//得到每个点之间的距离（是个小数）
	float x_data_offset, y_data_offset=point_distance;//x_data_offset存放x获取点距离开头的偏移量，y_data_offset存放y获取点距离开头的偏移量
	int i=0;//记录已经获取到几行

	while(cinfo.output_scanline < cinfo.output_height) 
	{    
		buffer = buf_save;//还原每一行的数据位置
		
		for( ;i<y_data_offset && cinfo.output_scanline < cinfo.output_height; i+=1)
		{
			jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&buffer, 1);
		}
		y_data_offset+=point_distance;

		/*
			x_data_offset=point_distance,先记录一开始的偏移量大小
			x_data_offset+=point_distance，每次都让偏移量加上每个点的距离
		*/
		for(x=x_s, x_data_offset=point_distance; x<x_s+cinfo.output_width*mul; x++, x_data_offset+=point_distance)
		{
			color = buffer[0]<<16 | buffer[1]<<8 |buffer[2];

			lcd_display_point(x, y, color, lcd_buf_ptr);

			//buf_save就是一行数据开头的位置再去加上每一个点的偏移量，(int)x_data_offset是为了保留偏移量的整数位，小数位不要
			buffer = buf_save+3*(int)x_data_offset;
		}

		y++;
	}

 

	jpeg_finish_decompress(&cinfo);
 
	jpeg_destroy_decompress(&cinfo);

 
	fclose(infile);

	return 1;
}


