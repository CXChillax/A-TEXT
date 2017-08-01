#include "head.h"
#include "gomoku.h"

void choose(void){
	printf("choose\n");
	set_xy_zero();
	lcd_draw_jpg(0, 0, "./pic/choose.jpg", NULL, 0, 0);
	while(1){
		if(ts_xy.x > 510 && ts_xy.x < 670 && ts_xy.y > 253 &&ts_xy.y < 312){
			printf("goto local_play\n");
			local_play();
		}
		if(ts_xy.x > 510 && ts_xy.x < 670 && ts_xy.y > 165 &&ts_xy.y < 226){
			printf("goto network_play\n");
			network_play();
		}
		if(ts_xy.x > 750 && ts_xy.x < 800 && ts_xy.y > 430 &&ts_xy.y < 480){
			printf("exit exe\n");
			exit_systeam();
		}
	}
	
}

void exit_systeam(void)
{
	printf("exit system\n");
	close_bgm();
	lcd_draw_jpg(0, 0, "./pic/quit.jpg", NULL, 0, 0);
	sleep(3);
	lcd_draw_jpg(0, 0, "./pic/b.jpg", NULL, 0, 0);
	//system("shut down");
	exit(0);
}

/************************
		本地
*************************/
void local_play(void)
{
	char gomoku[N][N];
	
	init_chessboard(gomoku);
	
	lcd_draw_jpg(0, 0, "./pic/gomoku.jpg", NULL, 0, 0);
	lcd_draw_jpg(175, 5, "./pic/gobang.jpg", NULL, 0, 0);
	
	int flag = 0;
	set_xy_zero();
	int m, n;
	int i, j;
	while(1){
	loop:	
		
		if(ts_xy.x >175&& ts_xy.x < 645&& ts_xy.y > 5 && ts_xy.y < 475){
			printf("===============\n");
			show_chessborad(gomoku, 190, 20, 40);
			printf("----------------\n");
			m = ts_xy.x;
			n = ts_xy.y;
			get_xy_by_tsxy(190, 20, &i, &j);
			if(gomoku[i][j] != '_'){
				set_xy_zero();
				goto loop;
			}
			
			printf("(i,j):(%d,%d)\n", i, j);
			
			if(flag == 0)
			{
				move_stone(gomoku, 190, 20, m, n, '1');
				flag = 1;
				if(checkWin(i, j, gomoku, '1'))
				{
					printf("white win\n");
					while(1){
						set_xy_zero();
						lcd_draw_jpg(0, 0, "./pic/white_win.jpg", NULL, 0, 0);
						if(ts_xy.x >300 && ts_xy.x < 350 && ts_xy.y > 155 && ts_xy.y < 200){
							local_play();
						}
						if(ts_xy.x >450 && ts_xy.x < 500 && ts_xy.y > 155 && ts_xy.y < 200){
							choose();
						}
					}
				}
			}else if(flag == 1)
			{
				move_stone(gomoku, 190, 20, m, n, '2');
				flag = 0;
				if(checkWin(i, j, gomoku, '2'))
				{
					printf("black win\n");
					while(1){
						set_xy_zero();
						lcd_draw_jpg(0, 0, "./pic/black_win.jpg", NULL, 0, 0);
						if(ts_xy.x >300 && ts_xy.x < 350 && ts_xy.y > 155 && ts_xy.y < 200){
							local_play();
						}
						if(ts_xy.x >450 && ts_xy.x < 500 && ts_xy.y > 155 && ts_xy.y < 200){
							choose();
						}
					}
					//break;
				}
			}
			
			set_xy_zero();
			//show_chessborad(gomoku, 190, 20, 40);
		}
		
	}
}


/************************
		联机
*************************/
void network_play(void){
	printf("in network_play\n");
	set_xy_zero();
	lcd_draw_jpg(0, 0, "./pic/ser_or_cli.jpg", NULL, 0, 0);
	int sockfd;
	char ip[20] = {0};
	char port[10] = {0};
	while(1){
		if(ts_xy.x > 190 && ts_xy.x < 350 && ts_xy.y > 210 &&ts_xy.y < 280){
			printf("ser_or_cli ser \n");
			printf("server input port\n");
			//scanf("%s", port);
			//keyboard(input)
			 sockfd = create_ser(atoi("5555"));
			if(sockfd < 0){
				perror("create_ser error");
				return;
			}
			struct sockaddr_in caddr; 
			socklen_t len = sizeof(caddr);
			server_play(sockfd, caddr, len);
		}
		if(ts_xy.x > 445 && ts_xy.x < 600 && ts_xy.y > 210 &&ts_xy.y < 280){
			printf("ser_or_cli cli\n");
			printf("client input server_ip and port\n");
			//scanf("%s%s", ip, port);
			//keyboard(input)
			sockfd = create_cli("192.168.15.225", atoi("5555"));
			if(sockfd < 0){
				perror("create_cli error");
				return ;
			}
			client_play(sockfd);
		}
		if(ts_xy.x > 750 && ts_xy.x < 800 && ts_xy.y > 430 &&ts_xy.y < 480){
			choose();
		}
	}
}

/************************
待实现的触摸屏输入ip和port

思路：根据触摸屏坐标得到数值
	  将数值转成字符串
*************************/
void keyboard(char *input){
	
}























