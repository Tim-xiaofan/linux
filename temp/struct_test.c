#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct node
{
	uint32_t i;//4
	uint8_t ch;//1 之后的三个字节不使用
	char *str;//8
	uint8_t* str1;//8
}test;//24 != 4 + 1 + 8 +8 = 21

int main(int argc, char* argv[])
{
	/*test *t;
	printf("before malloc size is %ld\n", sizeof(*t));
	test t1;
	printf("t1 size is %ld\n", sizeof(t1));
	t = &t1;
	printf("after & size is %ld\n", sizeof(*t));
	t->str = "Hello World\n";
	printf("after = size is %ld, str = %s\n", sizeof(*t), t->str);
	t = (test*)malloc(sizeof(test));
	printf("after malloc size is %ld, str = %s\n", sizeof(*t), t->str);*/
	test t;
	printf("sizeof(struct node) = %ld, sizeof(uint32_t) = %ld, sizeof(char*) = %ld, sizeof(uint8_t*) = %ld, sizeof(uint8_t) = %ld\n", 
	sizeof(struct node), sizeof(uint32_t), sizeof(char*), sizeof(uint8_t*), sizeof(uint8_t));
	printf("&t = %p\n&t.i = %p\n&t.ch = %p\n&t.str = %p\n&t.str1 = %p\n", &t, &t.i, &t.ch, &t.str, &t.str1);

	test* p;
}
