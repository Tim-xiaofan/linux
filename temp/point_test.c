#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node
{
	char c;
	int i;
	char str[3];
} test;

static void print_test(const test *t)
{
	if (t == NULL)
	{
		printf("valid test pointer\n");
		return;
	}
	printf("c = %c, i = %d, str = %s\n", t->c, t->i, t->str);
}

int main(int argc, char *argv[])
{
	test *pt = NULL;
	pt = (test *)malloc(sizeof(test));
	pt->c = 'c';
	pt->i = 7;
	pt->str[0] = 'a';
	pt->str[1] = 'b';
	pt->str[2] = '\0';
	test t;
	memcpy(&t, pt, sizeof(test));
	printf("&t = %p, pt = %p\n", &t, pt);
	print_test(&t);
	printf("after modify:\n");
	pt->c = 'f';
	pt->str[1] = 'h';
	print_test(&t);
	printf("after free:\n");
	free(pt);
	print_test(&t);
	print_test(pt);//c = , i = 0, str = ah(为啥)
}
