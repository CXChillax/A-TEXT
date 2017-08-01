#include "head.h"
#include "gomoku.h"

sem_t sem_pri;
sem_t sem_ts;

static int input_fd; 

//获取线程触摸屏坐标
 void *get_ts_xy(void *arg)
{
	struct input_xy *xy = (struct input_xy *)arg;
	while(1)
	{
		sem_wait(&sem_ts);
		*xy = input(input_fd);
		sem_post(&sem_pri);
	}
}

//终端输出屏幕坐标
void *pri_xy(void *arg)
{
	struct input_xy *xy = (struct input_xy *)arg;
	while(1)
	{
		sem_wait(&sem_pri);
		printf("(%d, %d)\n", xy->x, xy->y);
		sem_post(&sem_ts);
	}
} 

void *open_bgmusic(void *arg)
{
	system("madplay ./music/1.mp3 -r");
}

void close_bgm(void)
{
	system("killall -9 madplay");
}

int main(int argc, char **argv)
{
	// 打开lcd屏幕  打开触摸屏
	if(lcd_open() < 0)
	{
		perror("open lcd error\n");
		return -1;
	}		
	input_fd = open("/dev/event0",O_RDWR);
	if(input_fd == -1)
	{
		printf("intput_fd open failed\n");
		return -1;
	}
	
	//获取ts的坐标
	pthread_t id = 0;
	pthread_create(&id, NULL, get_ts_xy, (void *)&ts_xy);
	pthread_detach(id);
	pthread_create(&id, NULL, pri_xy, (void *)&ts_xy);
	pthread_detach(id);
	pthread_create(&id, NULL, open_bgmusic, NULL);
	pthread_detach(id);
	
	//信号量获取、输出ts坐标
	sem_init(&sem_ts, 0, 1);
    sem_init(&sem_pri, 0, 1);
	
	
	
	// 从开发板的（0,0）坐标开始显示启动界面
	show_gif("./pic/gif/214", 96);
	lcd_draw_jpg(0, 0, "./pic/startup.jpg", NULL, 0, 0);
	sleep(2);
	
	
	//local_play();
	
	choose();
	
	// int sockfd;
	// if(argc == 1){
		// printf("local_play\n");
		// local_play();
	// }
	// else if( argc == 2){
		// printf("server\n");
		// sockfd = create_ser(atoi(argv[1]));
		// if(sockfd < 0){
			// perror("create_ser error");
			// return;
		// }
		// struct sockaddr_in caddr; 
		// socklen_t len = sizeof(caddr);
		// server_play(sockfd, caddr, len);
	// }else if(argc == 3){
		// printf("client\n");
		// sockfd = create_cli(argv[1], atoi(argv[2]));
		// if(sockfd < 0){
			// perror("create_cli error");
			// return ;
		// }
		// client_play(sockfd);
	// }
	
	// 关闭显示屏
	//close(sockfd);
	close(input_fd);
	lcd_close();
	return 0;
}






















