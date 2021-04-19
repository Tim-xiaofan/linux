/** 20210116 11:05, zyj, GuangDong*/
//signal_multicall.c -- test multi call signal
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>


static bool force_quit = false;

void sighandler(int sig)
{
	if(sig == SIGINT)
	{
		printf("sighandler\n");
		force_quit = true;
	}
}

void sighandler1(int sig)
{
	if(sig == SIGINT)
	{
		printf("sighandler1\n");
		force_quit = true;
	}
}


int main()
{
	signal(SIGINT, sighandler);
	signal(SIGINT, sighandler1);

	while(!force_quit)
	{
		;
	}
	printf("\n");
	return 0;
}
