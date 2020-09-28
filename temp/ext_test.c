#include <stdio.h>
#include <string.h>

extern int ext;
extern void test();

int main(int argc, char *argv[])
{
    test();
    ext = 4;
    printf("in main ext = %d\n", ext);
}

//gcc -o ext_test ext_test.c file.c
