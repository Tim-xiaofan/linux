/** 20210324 10:49, zyj ,GuangDong*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>

int main()
{
	time_t  start,end;
	printf("sizeof(time_t) = %lu\n", sizeof(time_t));
	start =  time (NULL);
	printf("start = %lu\n", start);
	usleep(50);
	end =  time (NULL);
	printf ( "The pause used %f seconds.\n" ,  difftime (end,start));
	return  0;
}
