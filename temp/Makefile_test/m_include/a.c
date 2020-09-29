#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "a.h"
void a_func(void)
{
	time_t currentTime;
	currentTime = time(NULL);
	printf("A:%s\n", ctime(&currentTime));
}
