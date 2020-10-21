#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct test
{
	int len;
	void* data;
};

void print_arg(const void* arg)
{
	struct test* t = NULL;
	t = (struct test*) arg;
	printf("len = %d, data = %s\n", t->len, (char*)t->data);
}

int main(int argc, char* argv[])
{
	void* msg = NULL;
	char a[10];
	char* b = (char*)malloc(10);
	msg = a;
	printf("msg = a,  a is a[10]: sizeof (msg) is %ld, sizeof(*msg) = %ld,sizeof(a) = %ld, sizeof(a[5]) = %ld\n",
		       	sizeof(msg), sizeof(*msg), sizeof(a), sizeof(a[5]));
	msg = b;
	printf("msg = b,  a is (char*)malloc(10): sizeof (msg) is %ld, sizeof(*msg) = %ld,sizeof(b) = %ld, sizeof(b[5]) = %ld\n",
		       	sizeof(msg), sizeof(*msg), sizeof(b), sizeof(b[5]));
	struct test t = {2, "text"};
	print_arg((void*)&t);
}





