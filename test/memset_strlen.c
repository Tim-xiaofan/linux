#include <string.h>
#include <stdio.h>

int main()
{
	char str[16];
	memset(str, 0, 16);
	fprintf(stdout, "len = %lu\n", strlen(str));
}
