#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int x,y;
	printf("%s\n", "this is text!\n");
	//printf("%d\n", a);
	for (x = 1 ; x < 10; x++)
	{
		for (y = 1; y <= x; y++)
		{
			printf("%d * %d = %d\t", x, y, x*y);
			if(x==y)
				printf("\n");
		}
	}
	return 0;
}