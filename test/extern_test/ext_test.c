#include <stdio.h>
#include <string.h>

extern int ext;
extern int ext1;
extern void test();

int main(int argc, char *argv[])
{
    test();
    ext++;
    ext1++;
    printf("in main ext = %d ext1 = %d\n", ext, ext1);
}

//gcc -o ext_test ext_test.c file.c
