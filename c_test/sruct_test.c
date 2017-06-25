/*
	测试结构体的传值
	地址的传值
*/

#include <stdio.h>

struct btn
{
	int a;
	int b;
};


struct btn btns[] = {
	{1, 2},
	{3, 4},
	{5, 6},
};

int func(void *test)
{
	struct btn *p = (struct btn *)test;
	printf("%d\n", p->a);
	return 0;
}

int main(int argc, char const *argv[])
{
	for (int i = 0; i < sizeof(btns)/sizeof(btns[0]); ++i)
	{
		func((void *)(btns+i));
	}
	return 0;
}