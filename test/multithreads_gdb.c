#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void* PthreadRun1(void* arg)
{
	int count=5;
	while(count--){
		printf("Thread1::It's %ld pthread running...\n",pthread_self());
		sleep(1);
	}
	pthread_exit((void*)0);
}

void* PthreadRun2(void* arg)
{
	int count=5;
	while(count--){
		printf("Thread2::It's %ld pthread running...\n",pthread_self());
		sleep(1);
	}
	pthread_exit((void*)0);
}

int main()
{
	pthread_t id1,id2;

	if(0!=pthread_create(&id1,NULL,PthreadRun1,NULL))
	{
		printf("create id1 pthread error...\n");
		exit(1);
	}
	if(0!=pthread_create(&id2,NULL,PthreadRun2,NULL))
	{
		printf("create id2 pthread error...\n");
		exit(1);
	}
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	return 0;
}

