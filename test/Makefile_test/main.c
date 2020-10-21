#include <pthread.h>
#include <unistd.h>

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <a.h>
#include <b.h>
#include <c.h>

static void *test(void* argv)
{
	printf("I'm child\n");
	sleep(3);
	a_func();
	sleep(3);
	b_func();
	sleep(3);
	c_func();
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	printf("I'm main.c\n");
	sleep(3);
	a_func();
	sleep(3);
	b_func();
	sleep(3);
	c_func();
	pthread_t ptid;
	pthread_create(&ptid, NULL, test, NULL);
	pthread_join(ptid, NULL);
}
