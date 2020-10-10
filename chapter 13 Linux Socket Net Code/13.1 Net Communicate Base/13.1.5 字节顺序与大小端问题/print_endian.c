/*EXAMPLE
 *The program below display the results of converting an integer from host byte order to both little-endian and big-endian byte order.  Since host byte  order
 *is  either  little-endian  or  big-endian,  only  one  of these conversions will have an effect.  When we run this program on a little-endian system such as
 * x86-32, we see the following:
 * $ ./a.out
 * x.u32 = 0x44332211
 * htole32(x.u32) = 0x44332211
 * htobe32(x.u32) = 0x11223344
 * */
#include <stdio.h>
#include <endian.h>
#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * swap cpu-endian to big-endian and litlle-endian */
void swap(void)
{
	union {
		uint32_t u32;
		uint8_t arr[4];
	} x;

	x.arr[0] = 0x11;     /* Lowest-address byte */
	x.arr[1] = 0x22;
	x.arr[2] = 0x33;
	x.arr[3] = 0x44;     /* Highest-address byte */

	printf("x.u32 = 0x%x\n", x.u32);
	printf("htole32(x.u32) = 0x%x\n", htole32(x.u32));
	printf("htobe32(x.u32) = 0x%x\n", htobe32(x.u32));

	exit(EXIT_SUCCESS);
}


/**
 * 使用联合检测大小端*/
int check_cpu_endian()
{
	union word
	{
		int a;
		char b;
	}c;
	c.a = 1;
	return (c.b == 1);
}

int main(int argc, char* argv[])
{
	printf("Big-endian\t%d\nLittle-endian\t%d\nmine\t%d\n",
			BIG_ENDIAN, LITTLE_ENDIAN, BYTE_ORDER);
	if(check_cpu_endian() == 0)
		printf("cpu Big endian\n");
	else
		printf("cpu little endian\n");
	swap();
	return 0;
}
