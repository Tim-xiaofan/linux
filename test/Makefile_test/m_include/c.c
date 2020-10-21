#include "c.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
//#include "common.h"
void c_func(void)
{
	time_t currentTime;
	currentTime = time(NULL);
	printf("C:%s\n", ctime(&currentTime));
}
