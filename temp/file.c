#include <stdio.h>

int ext = 3;

void test()
{
    printf("int subfunc ext = %d\n", ext);
}

static void test2()
{
    //only use in this flie
}