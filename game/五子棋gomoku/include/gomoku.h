#ifndef __GOMOKU_H_
#define __GOMOKU_H_

//检测胜利
bool checkWin(int x,int y,char gomoku[N][N],char flagWin);

//初始化棋盘
void init_chessborad(char (*gomoku)[N]);

//显示棋子
void show_chessboard(char (*gomoku)[N], int start_x, int start_y, int offset);

//落子
bool move_stone(char (*array)[N], int start_x, int start_y,int ts_x, int ts_y, char key);

//获取落子的二维数组的位置
void get_xy_by_tsxy(int start_x, int start_y, int *a, int *b);

#endif
