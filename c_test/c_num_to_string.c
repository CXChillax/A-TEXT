#include <stdio.h>
#include <stdlib.h>


//识别百位或个位
void hunderd(int hun)
{
	switch(hun)
	{
		case(1):
			printf(" One");
			break;
		case(2):
			printf(" Two");
			break;
		case(3):
			printf(" Three");
			break;
		case(4):
			printf(" Four");
			break;
		case(5):
			printf(" Five");
			break;
		case(6):
			printf(" Six");
			break;
		case(7):
			printf(" Seven");
			break;
		case(8):
			printf(" Eight");
			break;
		case(9):
			printf(" Nine");
			break;
		default:
			break;											 
	}
}

//十位
void ten(int a)
{
	switch(a)
	{
		case(2):
			printf(" Twenty");
			break;
		case(3):
			printf(" Thirty");
			break;
		case(4):
			printf(" Forty");
			break;
		case(5):
			printf(" Fifty");
			break;
		case(6):
			printf(" Sixty");
			break;
		case(7):
			printf(" Seventy");
			break;
		case(8):
			printf(" Eighty");
			break;
		case(9):
			printf(" Ninty");
			break;
		default:
			break;											 
	}
}

//只有十位和个位的情况
void fun(int x, int y)
{
	switch(10*x+y)
	{
		case(10):
			printf(" Ten");
			break;
		case(11):
			printf(" Eleven");
			break;
		case(12):
			printf(" Twelve");
			break;
		case(13):
			printf(" Thirteen");
			break;
		case(14):
			printf(" Fourteen");
			break;
		case(15):
			printf(" Fifteen");
			break;
		case(16):
			printf(" Sixteen");
			break;
		case(17):
			printf(" Seventeen");
			break;
		case(18):
			printf(" Eighteen");
			break;
		case(19):
			printf(" Nineteen");
			break;
		default:
			break;											 
	}
}

void fun_2(int a, int b, int c)
{
	if(a>0)
	{
		hunderd(a);
		printf(" hunderd and ");
	}
	if (b == 0)
		hunderd(c);
	if (b==1)
	{
		fun(b, c);
	}
	if ((b>1)&&(b<=9))
	{
		ten(a);
		hunderd(c);
	}
	printf("\n");
}

int main(int argc, char const *argv[])
{
	int num;
	int hun, ten, unit;
	scanf("%d", &num);
	if(num<0&&num>=1000)
	{
		printf("error\n");
		return -1;
	}
	else
	{
		hun = num/100;
		ten = num/10%10;
		unit = num%10;

		fun_2(hun, ten, unit);
	}
	return 0;
}