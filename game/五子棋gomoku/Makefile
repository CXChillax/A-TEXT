all:
	arm-linux-gcc ./lib/*.c *.c -o main -L./include -I./include -L./libjpeg -L./libts -I./libjpeg -I./libts -ljpeg -lts -lapi_v4l2_arm -lpthread
clean:
	rm *.o main
