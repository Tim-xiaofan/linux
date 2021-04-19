/** 20210419 12:39, zyj ,GuangDong*/
#include <stdio.h>
#include <stdlib.h>

int A(int i, float f);
int B(int i, float f);

int main()
{
	A(1, 3.14);
	return 0;
}

int A(int i, float f)
{
	return B(i, f);
}


int B(int i, float f)
{
	return i;
}
