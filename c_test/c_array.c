#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	//声明玩家，赢家
	int player = 0;
	int winner = 0;
	int choice = 0;

	unsigned int row , column;
	row = column = 0;

	int count = 9;

	//井字过三关
	char board[][3]  = {
		{'1', '2', '3'},
		{'4', '5', '6'},
		{'7', '8', '9'},
	};

	//有人胜出或者超过9次，结束游戏
	for (unsigned int i = 0; i < count && (winner == 0); ++i)
	{
		printf("\n%d\n", i+1);
		printf(" %c | %c | %c\n", board[0][0], board[0][1], board[0][2]);
		printf("---+---+---\n");
		printf(" %c | %c | %c\n", board[1][0], board[1][1], board[1][2]);
		printf("---+---+---\n");
		printf(" %c | %c | %c\n", board[2][0], board[2][1], board[2][2]);
		printf("\n");

		//选择玩家
		player = i%2 +1;

		do
		{
			printf("player %d, please enter a valid aquare number for where you want to place your %c ", player, (player == 1)? 'X':'O');

			scanf("%d", &choice);

			row = --choice / 3;
			column = choice % 3;

		}while(choice <0 || choice >8 || board[row][column] > '9');

		board[row][column] = (player == 1)? 'X': 'O';

		//检查获胜的判断
		if((board[0][0]==board[1][1]&&board[0][0]==board[2][2])||(board[0][2]==board[1][1]&&board[0][2]==board[2][0]))
		{
			winner = player;
		}
		else
		{
			for (unsigned int line = 0; line <= 2; ++line)
			{
				if((board[line][0]==board[line][1]&&board[line][0]==board[line][2])||(board[0][line]==board[1][line]&&board[0][line]==board[2][line]))
				{
					winner = player;
				}
			}
		}
	}

	printf(" %c | %c | %c\n", board[0][0], board[0][1], board[0][2]);
	printf("---+---+---\n");
	printf(" %c | %c | %c\n", board[1][0], board[1][1], board[1][2]);
	printf("---+---+---\n");
	printf(" %c | %c | %c\n", board[2][0], board[2][1], board[2][2]);
	printf("\n");

	if (winner)
	{
		printf("Congratulation, player %d, you are the winner\n", winner);
	}
	else
	{
		printf("How boring , it is draw\n");
	}

	return 0;
}