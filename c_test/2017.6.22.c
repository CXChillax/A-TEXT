#include <stdio.h>

void change(int k[])
{
	k[0] = k[5];
}

int main(int argc, char const *argv[])
{
	int x[10] = {1,2,3,4,5,6,7,8,9,10};
	int n = 0;
	while(n<=4)
	{
		change(&x[n]);
		n++;	
	}
	for (int i = 0; i < 5; ++i)
		{
			printf("%d\n", x[i]);
			printf("\n");
		}
	return 0;
}