#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "curl/curl.h"
#define MAX_BUF      65536

char wr_buf[MAX_BUF+1];
int  wr_index; 

//这个函数是为了符合CURLOPT_WRITEFUNCTION而构造的
//完成数据保存功能
size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    int segsize = size * nmemb;
 
    /* Check to see if this data exceeds the size of our buffer. If so,
    * set the user-defined context value and return 0 to indicate a
    * problem to curl.
    */
    if (wr_index + segsize > MAX_BUF) {
        *(int *)userp = 1;
        return 0;
    }
 
    /* Copy the data from the curl buffer into our buffer */
    memcpy((void *)&wr_buf[wr_index], buffer, (size_t)segsize);
 
    /* Update the write index */
    wr_index += segsize;
 
    /* Null terminate the buffer */
    wr_buf[wr_index] = 0;
 
    /* Return the number of bytes received, indicating to curl that all is okay */
    return segsize;
}

// 获取今天的天气
void get_today(void)
{
	cJSON *json; // 临时的json指针
	
	// 将获取的字符串数据，转成JSON格式
	json = cJSON_Parse(wr_buf);
	// 查询键值函数，通过json指针，得到键值
	// 根据键值找到数组，获取数组的数据
	
	
}

int main(void)
{
	// 定义一个curl指针
	CURL *curl;
	int ret = -1;
	int wr_error = 0;
	
	// 1.初始化curl -->调用curl_easy_init()函数得到 easy interface型指针
	curl = curl_easy_init();
	if(!curl)
	{
		printf("curl 初始化失败\n");
		return -1;
	}
	
	// 查询第三个参数的网址
	curl_easy_setopt(curl, CURLOPT_URL, "http://t.weather.sojson.com/api/weather/city/101040100");
	
	/* 使用回调函数并传递一个错误指针 */
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&wr_error);
	// 调用回调函数，将网页数据存放在wr_buf，默认不在打印
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	
	// 获取返回结果,如果正确为0
	ret = curl_easy_perform(curl);
	printf("ret = %d\n", ret);
	if(ret != 0)
		printf("获取网址错误\n");
	printf("网页数据-->%s\n", wr_buf);
	
	// 整理数据 --》cJSON
	
	
	
	// 释放curl指针，释放内存
	curl_easy_cleanup(curl);
	
	
	return 0;
}