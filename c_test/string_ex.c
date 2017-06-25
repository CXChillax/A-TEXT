#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TEXT_LEN 10000
#define BUFSIZE 100
#define MAX_WORDS 500
#define WORD_LEN 12

int main(int argc, char const *argv[])
{
	//init
	char delimiters[] = "\n\".,;:!?)(";
	char text[TEXT_LEN] = "";		//{0}
	char buf[BUFSIZE];
	char words[MAX_WORDS][WORD_LEN];
	int nword[MAX_WORDS] = {0};
	int word_count = 0;

	puts("Enter text on an arbitrary number of lines");
	puts("Enter an empty line to end input:");

	//deal whit arbitrary number of lines of text
	while(true)
	{
		fgets(buf, BUFSIZE, stdin);

		if(buf[0]=='\0')
			break;

		//拼接字符串和检查有没有足够的剩余空间
		if(strcat_s(text, TEXT_LEN, buf))		//strncat(text, buf, TEXT_LEN)
		{
			puts("已经达到最大值，程序退出");
			return 1;
		}

		//查找第一个单词
		size_t len = TEXT_LEN;
		char *ptr = NULL;
		char *pWord = strtok_s(text, &len, delimiters, &ptr);

		if(!pWord)
		{
			printf("no words fond. Ending program\n");
			return 1;
		}

		strcpy_s(words[word_count], WORD_LEN, pWord);		//strncpy(words[word_count], pWord, WORD_LEN) char *strncpy(char *dest, const char *src, size_t n);
		++nword[word_count++];

		//查找剩下部分的word
		bool new_word = true;		//false 表示有存在的单词
		while(true)
		{
			pWord = strtok_s(NULL, &len, delimiters, &ptr);		//查找随后的单词
			if(!pWord)
				break;

			//检查存在的单词
			for (int i = 0; i < word_count; ++i)
			{
				if(strcmp(words[i], pWord) == 0)
				{
					++nword[i];
					new_word = false;
				}
			}

			if(new_word)
			{
				strcpy_s(words[word_count], WORD_LEN, pWord);		//copy to array
				++nword[word_count++];
			}
			else
				new_word = true;

			if(word_count > MAX_WORDS -1)
			{
				puts("Capacity to store words exceeded. \n");
				return 1;
			}
		}

		//list the words
		for (int x = 0; x < word_count; ++x)
		{
			printf("%-13s %3d", words[x], nword[x]);
			if((x+1)%4 == 0)
				printf("\n");
		}
	}
		printf("\n");

	return 0;
}