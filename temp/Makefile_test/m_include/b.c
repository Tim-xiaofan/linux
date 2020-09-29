#include "b.h"
//#include "common.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
void b_func(void)
{
	time_t currentTime;
	currentTime = time(NULL);
	printf("B:%s\n", ctime(&currentTime));
}
