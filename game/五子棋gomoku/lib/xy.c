#include "head.h"

/**********************
	功能：触摸屏获取坐标
	fun：get ts (x,y)
	para: int input_fd	设备的文件描述符
	return : struct input_xy
***********************/
struct input_xy input(int input_fd)
{
	struct input_xy point;
	struct input_event xy;
	
	int x_read = true;//1
	int y_read = false;//0
	while(1)
	{
		//读取触摸屏的数据 并将其存到struct input_event 类型的变量 xy
		read(input_fd,&xy,sizeof(xy));
		
		//printf("---%d---%d----%d---\n",xy.type,xy.code,xy.value);
		if(xy.type == EV_ABS  &&xy.code == ABS_X && x_read == true)
		{
			point.x = xy.value;
			x_read = false;
			y_read = true;
			
		}
		if(xy.type == EV_ABS  &&xy.code == ABS_Y && y_read == true)
		{
			point.y = xy.value;
			y_read = false;
		}
		if(xy.type == EV_ABS && xy.code == ABS_PRESSURE && xy.value == 0 && x_read ==false && y_read == false)
			break;
	}
	return point;
}

/********************
	将全局坐标置0；
********************/
void set_xy_zero(void)
{
	ts_xy.x = 0;
	ts_xy.y = 0;
}
