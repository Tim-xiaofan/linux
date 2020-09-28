#include <stdio.h>
#include <string.h>

//extern ext;

int main(int argc, char *argv[])
{
    //printf("in main ext = %d\n", ext);
    //test();
    char *a = "abcd";
    char b[10];
    memcpy(b, a, strlen(a) + 1);
    b[0] = '1';
    printf("a = %s\n", a);
    printf("b = %s\n", b);
}

//gcc -o test test.c file.c
