#include <stdio.h>
#define LOG(fmt, args...) printf("[%s:%d] "fmt, __FILE__, __LINE__ , ##args)
#define LOGi(fmt, arg1) printf("[%s:%d] "fmt"\n", __FILE__, __LINE__ , arg1)
#define LOGii(fmt, arg1, arg2) printf("[%s:%d] "fmt"\n", __FILE__, __LINE__ , arg1, arg2)

void test(const char* info);
int main()
{
	LOG("TEST\n");
	LOGi("TEST%d", 1);
	LOGii("TEST%d,%d", 1, 2);
	test("TEST%d");
}

void test(const char* info)
{
	int i = 12;
	LOGi(info, i);
}
