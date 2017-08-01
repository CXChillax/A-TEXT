#ifndef __HEAD_H_
#define __HEAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <stdbool.h>

#include <fcntl.h>		//O_RDWR
#include <linux/input.h>	//input child system	event EV_ABS

#include <sys/types.h>		
#include <sys/socket.h>		//TCP UDP ...
#include <netinet/in.h>		//inet_addr ...
#include <arpa/inet.h>		//htons..

#include <pthread.h>
#include <semaphore.h>

#define CHESSBOARD_W	11	//	棋盘长宽		好像没用到 - -!!
#define CHESSBOARD_H	11

#define N	11		//二维数组的大小 和棋盘长宽相对

struct input_xy {
	int x;
	int y;
}ts_xy;

extern struct input_xy ts_xy;

//获取触摸屏坐标
struct input_xy input(int input_fd);

void set_xy_zero(void);

void local_play(void);

void network_play(void);

void choose(void);

void exit_systeam(void);

void close_bgm(void);

#endif
