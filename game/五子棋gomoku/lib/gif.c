#include "head.h"

void show_gif(char *path, int cout){
	int i;
	char buf[128] = {0};
	for(i = 1; i <= cout; i++){
		//usleep(500);
		sprintf(buf, "%s/%d.jpg", path, i);
		puts(buf);
		lcd_draw_jpg(0, 0, buf, NULL, 0, 0);
	}
}
