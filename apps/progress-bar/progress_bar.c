/* 20210313 21:00, zyj, GuangDong*/
//progress_bar.cpp -- app to show progress bar
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define BAR_LEN 101
#define FMT "[%-100s][%d%%]\t\t%d KB/s\r"

int main()
{
	int i, delay;
	char bar[BAR_LEN];
	for(i = 0; i < BAR_LEN; i++)
		bar[i] = '\0';
	srand(time(NULL));

	for(i = 0; i < BAR_LEN; i++)
	{
		delay = 100000 + (rand() % 300000); 
		printf(FMT, bar, i, delay);
		fflush(stdout);
		bar[i] = '#';
		usleep(delay);
	}
	printf("\n");
}
