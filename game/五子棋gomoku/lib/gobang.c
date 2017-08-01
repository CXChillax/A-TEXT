#include "head.h"

/******************
**	name: init_chessborad()
**	作用：初始化棋盘
**	参数：
**	char (*gomoku)[N]	一个棋盘的二维数组
*******************/
void init_chessboard(char gomoku[N][N]){
	int x,y;
	for(x = 0; x<N; x++)
		for(y = 0; y<N; y++)
		{
			gomoku[x][y] = '_';
		}
			
}

/******************
**	name: show_chessborad()
**	作用：根据二维数组 将落子情况展示
**	参数： 
**		char (*gomoku)[N]:
**		(start_x,start_y): 从哪里开始显示落子
**		int offset : 根据棋子大小偏移
*******************/
void show_chessborad(char (*gomoku)[N], int start_x, int start_y, int offset){
	int x, y;
	int temp = start_x;
	for(x =0 ; x < N; x++){
		for(y = 0; y < N; y++){
			if(gomoku[x][y] == '1'){
				lcd_draw_jpg(start_x, start_y, "./pic/white.jpg", NULL, 0, 0);
			}
			if(gomoku[x][y] == '2'){
				lcd_draw_jpg(start_x, start_y, "./pic/black.jpg", NULL, 0, 0);
			}
			start_x += offset;
		}
		start_y += offset;
		start_x = temp;
	}
}

/******************
**	name: fouces
**	作用： 在特定位置显示特定棋子
**	参数： 
**		char (*gomoku)[N]:
**		(start_x,start_y): 从哪里开始显示落子
**		int offset : 根据棋子大小偏移
**		(gom_x, gom_y): 二维数组的下标
*******************/
void fouces(char (*gomoku)[N], int start_x, int start_y, int gom_x, int gom_y, int offset){
	int x, y;
	int temp = start_x;
	for(x =0 ; x < N; x++){
		for(y = 0; y < N; y++){
			if((gomoku[x][y] == '1') &&(gom_x == x && gom_y ==y )){
				printf("fouces(x,y)(%d,%d) start_x, start_y(%d,%d)\n", x, y, start_x, start_y);
				lcd_draw_jpg(start_x, start_y, "./pic/white_f.jpg", NULL, 0, 0);
			}
			if( (gomoku[x][y] == '2')&&(gom_x == x &&gom_y == y)){
				printf("fouces(x,y)(%d,%d) start_x, start_y(%d,%d)\n", x, y, start_x, start_y);
				lcd_draw_jpg(start_x, start_y, "./pic/black_f.jpg", NULL, 0, 0);
			}
			//start_x += offset;
		}
		//start_y += offset;
		//start_x = temp;
	}
}

/******************
**	name : lock_chess()
**	作用 ：将已经下了的棋锁定
**	参数 ： 
**		char (*array[N])	要锁的棋盘
**		char (*p)[N]		保存锁的状态
*******************/
/*
void lock_chess(char (*array)[N], char (*p)[N]){
	int x,y;
	for(x = 0; x < N; x++)
	{
		for(y = 0; y < N; y++)
		{
			if(array[x][y] == '0'){
				p[x][y] = '0';
			}else{
				p[x][y] = '1';
			}
		}
	}
}
*/


/******************
**	name : move_stone()
**	作用 : 根据坐标和棋盘状态下棋
**	参数 :
**		char (*array[N])	原棋盘
**		int start_x，int start_y	棋子开始展示的坐标
**		int ts_x, int ts_y			落子的坐标
**		char key					落子类型
&&(array[x][y] == '0')
*******************/
bool move_stone(char (*array)[N], int start_x, int start_y,int ts_x, int ts_y, char key){
	int x, y;
	int temp = start_x;
	for(x = 0; x < N; x++){
		for(y = 0; y < N; y++){
			if((ts_x > start_x  && ts_x < start_x + 40)&&(ts_y >440 - start_y  && ts_y < 440 - start_y + 40)&&(array[x][y] == '_')){
				//printf("(x,y)(%d,%d) start_x, start_y(%d,%d)\n", x, y, start_x, start_y);
				if(key == '1'){
					//lcd_draw_jpg(start_x, start_y, "./pic/white.jpg", NULL, 0, 0);
					array[x][y] = key;
					fouces(array, start_x, start_y, x, y, 40);
					return true;
				}
				if(key == '2'){
					//lcd_draw_jpg(start_x, start_y, "./pic/black.jpg", NULL, 0, 0);
					array[x][y] = key;
					fouces(array, start_x, start_y, x, y, 40);
					return true;
				}
			}
			start_x += 40;
		}
		
		start_y += 40;
		start_x = temp;
	}
}

/******************
**	name: get_xy_by_tsxy()
**	作用： 根据触摸屏坐标得到哪个x,y
**	参数：
**		int start_x, int start_y
**		int *a, int *b		获取到的xy
*******************/

void get_xy_by_tsxy(int start_x, int start_y, int *a, int *b){
	int x, y;
	int temp = start_x;
	for(x = 0; x < N; x++){
		for(y = 0; y < N; y++){
			if((ts_xy.x > start_x && ts_xy.x < start_x + 40)&&(ts_xy.y >440 - start_y && ts_xy.y < 440 - start_y + 40)){
				
				printf("get gobang[][]  is[%d][%d]\n", x, y);
				*a = x;
				*b = y;
			}
			start_x += 40;
		}
		start_y += 40;
		start_x = temp;
	}
}

/******************
**	name:	checkWin（）
**	参数:
**	(x, y) ——>坐标
**	gomoku ——>被判断的二维数组
**	flagWin ——>字符型的胜利标志
**	return :	true or false
**	思想：从选点遍历四个 有则胜利条件加一，没有则直接跳出不遍历
*******************/
bool checkWin(int x,int y,char gomoku[N][N],char flagWin){
	int i, j;
	i = j = 0;
	int  win=0;
	
	//左到右
	for(i=y;i<N;i++)//->
		if(gomoku[x][i]==flagWin)
			win++;
		else break;
	//右到左
	for(i=y-1;i>=0;i--)
		if(gomoku[x][i]==flagWin)
			win++;
		else break;
	if(win>=5) return true;
	win=0;

	//上到下
	for(i=x;i<N;i++)
		if(gomoku[i][y]==flagWin)
			win++;
		else break;
	//下到上
	for(i=x-1;i>=0;i--)
		if(gomoku[i][y]==flagWin)
			win++;
		else break;
	if(win>=5) return true;
	win=0;


	//右下
	for(i=x,j=y;i<N,y<N;i++,j++)
		if(gomoku[i][j]==flagWin)
			win++;
		else break;
	//左上
	for(i=x-1,j=y-1;i>=0,j>=0;i--,j--)
		if(gomoku[i][j]==flagWin)
			win++;
		else break;
	if(win>=5) return true;
	win=0;


	//右下
	for(i=x,j=y;i>=0,j<N;i--,j++)
		if(gomoku[i][j]==flagWin)
			win++;
		else break;
	//左上
	for(i=x-1,j=y-1;i<N,j>=0;i++,j--)
		if(gomoku[i][j]==flagWin)
			win++;
		else break;
	if(win>=5) return true;
	return false;
}