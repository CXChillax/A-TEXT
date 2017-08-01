#include "head.h"
#include "gomoku.h"

/**********************
**	创建一个服务端
**********************/

int create_ser(int prot){
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("ser sockfd fail");
		return -1;
	}
	
	int i = 1;/* 允许重复使用本地地址与套接字进行绑定 */
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));
	
	struct sockaddr_in saddr;
	bzero(&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(prot); 
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int ret = bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
	if(ret < 0){
		perror("bind error");
		return -2;
	}
	
	ret = listen(sockfd, 5);
	if(ret < 0){
		perror("listen error");
		return -3;
	}
	
	return sockfd;
}

/**********************
**	创建一个客户端
**********************/
int create_cli(char *ip, int prot){
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd < 0){
		perror("ser sockfd fail");
		return -1;
	}
	
	struct sockaddr_in saddr;
	bzero(&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(prot); 
	saddr.sin_addr.s_addr = inet_addr(ip);
	
	int ret = connect(sockfd, (struct sockaddr*)&saddr, sizeof(saddr));
	if(ret < 0)
	{
		perror("connect error");
		return -2;
	}
	return sockfd;
}

/*********************
**	name: server_play()
**	作用： 作为服务端先手
**	参数：	以下参数都是socket（）要用到的 所以.....
**		sockfd, 			
**		struct sockaddr_in caddr, 
**		socklen_t len
*********************/

void server_play(int sockfd, struct sockaddr_in caddr, socklen_t len){
	
	int msg[2];
	char gomoku[N][N];
	memset(gomoku, 0, sizeof(gomoku));
	
	init_chessboard(gomoku);
	
	printf("wait cli\n");
	int clientfd = accept(sockfd, (struct sockaddr *)&caddr, &len);
	if(clientfd < 0){
		perror("accept error");
		exit(-1);
	}
	
	lcd_draw_jpg(0, 0, "./pic/gomoku.jpg", NULL, 0, 0);
	lcd_draw_jpg(175, 5, "./pic/gobang.jpg", NULL, 0, 0);
	
	int ret;
	int m, n, i, j;
	
	int flag = 0;
	set_xy_zero();
	while(1){
		
	loop:	
		if(ts_xy.x >175&& ts_xy.x < 645&& ts_xy.y > 5 && ts_xy.y < 475){
			//show_chessborad(gomoku, 190, 20, 40);
			printf("===============\n");
			m = ts_xy.x;
			n = ts_xy.y;
			
			//获取二维数组的位置
			get_xy_by_tsxy(190, 20, &i, &j);
			if(gomoku[i][j] != '_'){
				set_xy_zero();
				goto loop;
			}
			
			//下棋到棋盘
			printf("(i,j):(%d,%d)\n", i, j);
			move_stone(gomoku, 190, 20, m, n, '1');
			
			msg[0] = i;
			msg[1] = j;
			
			//将服务器下的棋发送到客户端
			ret = send(clientfd, &msg, sizeof(msg), 0);
			
			if(ret < 0){
				perror("ser send");
				break;
			}
			
			//检测胜利 服务端
			if(checkWin(i, j, gomoku, '1')){
				printf("white win\n");
				while(1){
					set_xy_zero();
					lcd_draw_jpg(0, 0, "./pic/white_win.jpg", NULL, 0, 0);
					
					//选择是否继续
					if(ts_xy.x >300 && ts_xy.x < 350 && ts_xy.y > 155 && ts_xy.y < 200){
						
						//重新初始化棋盘
						memset(gomoku, 0, sizeof(gomoku));
						init_chessboard(gomoku);
						lcd_draw_jpg(0, 0, "./pic/gomoku.jpg", NULL, 0, 0);
						lcd_draw_jpg(175, 5, "./pic/gobang.jpg", NULL, 0, 0);
						flag = 0;
						set_xy_zero();
						goto loop;
					}
					if(ts_xy.x >450 && ts_xy.x < 500 && ts_xy.y > 155 && ts_xy.y < 200){
						choose();
					}
				}
			}
			
			set_xy_zero();
			flag = 1;
		}
		
	again:	
		if(flag == 1){
			printf("wait client\n");
			recv(clientfd, &msg, sizeof(msg), 0);  
			gomoku[msg[0]][msg[1]] = '2';
			
			//在服务端检测黑子
			if(checkWin(msg[0], msg[1], gomoku, '2')){
				printf("black win\n");
				while(1){
					set_xy_zero();
					lcd_draw_jpg(0, 0, "./pic/black_win.jpg", NULL, 0, 0);
					
					//选择是否继续
					if(ts_xy.x >300 && ts_xy.x < 350 && ts_xy.y > 155 && ts_xy.y < 200){
						
						//重新初始化棋盘
						memset(gomoku, 0, sizeof(gomoku));
						init_chessboard(gomoku);
						lcd_draw_jpg(0, 0, "./pic/gomoku.jpg", NULL, 0, 0);
						lcd_draw_jpg(175, 5, "./pic/gobang.jpg", NULL, 0, 0);
						flag = 1;
						set_xy_zero();
						goto again;
					}
					if(ts_xy.x >450 && ts_xy.x < 500 && ts_xy.y > 155 && ts_xy.y < 200){
						choose();
					}
				}
				//break;
			}
			show_chessborad(gomoku, 190, 20, 40);
			set_xy_zero();
			flag = 0;
		}
		
	}
	
	printf("cse\n");
	
	close(clientfd);
	
}

/*********************
**	name: local_play()
**	作用： 作为服务端先手
**	参数：	以下参数都是socket（）要用到的 所以.....
**		sockfd, 			
*********************/

void client_play(int sockfd){
	
	int msg[2];
	char gomoku[N][N];
	memset(gomoku, 0, sizeof(gomoku));
	
	init_chessboard(gomoku);
	
	lcd_draw_jpg(0, 0, "./pic/gomoku.jpg", NULL, 0, 0);
	lcd_draw_jpg(175, 5, "./pic/gobang.jpg", NULL, 0, 0);
	
	int ret;
	int m, n, i, j;
	
	int flag = 1;
	set_xy_zero();
	while(1){
		
	again:	
		if( flag == 1){
			printf("wait ser\n");
			recv(sockfd, &msg, sizeof(msg), 0);
			gomoku[msg[0]][msg[1]] = '1';
			
			//在客户端检测白子是否胜利
			if(checkWin(msg[0], msg[1], gomoku, '1')){
				printf("white win\n");
				while(1){
					set_xy_zero();
					lcd_draw_jpg(0, 0, "./pic/white_win.jpg", NULL, 0, 0);
					
					//选择是否继续
					if(ts_xy.x >300 && ts_xy.x < 350 && ts_xy.y > 155 && ts_xy.y < 200){
						
						//重新初始化棋盘
						memset(gomoku, 0, sizeof(gomoku));
						init_chessboard(gomoku);
						lcd_draw_jpg(0, 0, "./pic/gomoku.jpg", NULL, 0, 0);
						lcd_draw_jpg(175, 5, "./pic/gobang.jpg", NULL, 0, 0);
						flag = 1;
						set_xy_zero();
						goto again;
					}
					if(ts_xy.x >450 && ts_xy.x < 500 && ts_xy.y > 155 && ts_xy.y < 200){
						choose();
					}
				}
			}
			show_chessborad(gomoku, 190, 20, 40);
			set_xy_zero();
			flag = 0;
		}
		
	loop:	
		if(ts_xy.x >175&& ts_xy.x < 645&& ts_xy.y > 5 && ts_xy.y < 475){
			printf("===============\n");
			m = ts_xy.x;
			n = ts_xy.y;
			get_xy_by_tsxy(190, 20, &i, &j);
			if(gomoku[i][j] != '_'){
				set_xy_zero();
				goto loop;
			}
			
			printf("(i,j):(%d,%d)\n", i, j);
			move_stone(gomoku, 190, 20, m, n, '2');
			
			msg[0] = i;
			msg[1] = j;
			
			ret = send(sockfd, &msg, sizeof(msg), 0);
			
			if(ret < 0){
				perror("ser send");
				break;
			}
			
			if(checkWin(i, j, gomoku, '2')){
				printf("black win\n");
				while(1){
					set_xy_zero();
					lcd_draw_jpg(0, 0, "./pic/black_win.jpg", NULL, 0, 0);
					
					//选择是否继续
					if(ts_xy.x >300 && ts_xy.x < 350 && ts_xy.y > 155 && ts_xy.y < 200){
						
						//重新初始化棋盘
						memset(gomoku, 0, sizeof(gomoku));
						init_chessboard(gomoku);
						lcd_draw_jpg(0, 0, "./pic/gomoku.jpg", NULL, 0, 0);
						lcd_draw_jpg(175, 5, "./pic/gobang.jpg", NULL, 0, 0);
						flag = 0;
						set_xy_zero();
						goto again;
					}
					if(ts_xy.x >450 && ts_xy.x < 500 && ts_xy.y > 155 && ts_xy.y < 200){
						choose();
					}
				}
				//break;
			}
			
			set_xy_zero();
			flag = 1;
		}
	}
	
	printf("cli\n");
	
	
	///???????????
	close(sockfd);
}












































