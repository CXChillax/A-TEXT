/****************************************************************************************
 *项目名称:触摸屏封装好的函数
 *设    计:温子祺
 *开 发 板:gec210开发板
 *日    期:2015-12-25
 *说	明:使用了tslib
****************************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include "tslib.h"

static struct tsdev *g_ts=NULL;

/* 采样结果 */
static struct ts_sample g_ts_sample;

int ts_open_ex(void)
{
	/* 打开触摸屏设备 */
	g_ts= ts_open("/dev/event0",0);
	
	if(g_ts == NULL)
		return -1;
	 
	ts_config(g_ts);	
	
	return 0;
}

int ts_xy_get(int *x,int *y,int is_wait_release)
{
	int rt=-1;
	
	rt=ts_read(g_ts,&g_ts_sample,1);
	
	if(rt < 0)
		return -1;
	
	while(is_wait_release)
	{
		ts_read(g_ts,&g_ts_sample,1);
		
		if(g_ts_sample.pressure == 0)
			break;
		
	}
	
	*x = g_ts_sample.x;
	*y = g_ts_sample.y;	
	
	return 0;	
}

int ts_is_release(void)
{
	ts_read(g_ts,&g_ts_sample,1);
	
	printf("pressure=%d\r\n",g_ts_sample.pressure);
	
	if(g_ts_sample.pressure == 0)
		return 0;
	
	return 1;	
}

int ts_close_ex(void)
{
	if(g_ts)
		ts_close(g_ts);
	
	return 0;	
}
