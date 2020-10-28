/*test byte align in gcc 32-bit*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct A
{
    int i;//4
    char c;//1
    short s;//2
};

/** 0x1c=28
 * size(struct xx) = 12
 * 0x7fffa1eb191c
 * 0x7fffa1eb191c
 * 0x7fffa1eb1920
 * 0x7fffa1eb1924
 * 默认指定对齐为4*/
struct B
{
    char c;//1 0x7fffa1eb191c%1=0 28~29
    int i;//4 0x7fffa1eb1920%4!=0 32~35 
    short s;//2 0x7fffa1eb1924%2=0 36~37  
    /*故28~37共10个字节，可是实际是12个字节：
     * 根据结构体圆整的要求，28到37=10字节，（10＋2）％4＝0。
     * 所以38到39也为结构体B所占用。故B从28到39共有12个字节,sizeof(struct B)=12;
     * */
};

struct B1
{
	char c;
	int i;
	short s;
	short s1;
	/**
	 * 猜测12字节
	 * 实际运行结果12字节*/
};

/**size(struct xx) = 8:
 * 0x7ffd21d3ac2c
 * 0x7ffd21d3ac2c
 * 0x7ffd21d3ac30
 * 0x7ffd21d3ac34
 * 默认指定对齐设置为2*/
#pragma pack (2) /*指定按2字节对齐*/
struct C
{
    char c;
    int i;
    short s;
};
#pragma pack () /*取消指定对齐，恢复缺省对齐*/

char fmt[] = "size(struct xx) = %ld:\n%p\n%p\n%p\n%p\n";

int main(int argc, char* argv[])
{
	struct A ma;
	//printf(fmt, sizeof(struct A), &ma, &ma.i, &ma.c, &ma.s);
	struct B mb;
	printf(fmt, sizeof(struct B), &mb, &mb.c, &mb.i, &mb.s);
	struct C  mc;
	printf(fmt, sizeof(struct C), &mc, &mc.c, &mc.i, &mc.s);
	struct B1 mb1;
	printf(fmt, sizeof(struct B1), &mb1, &mb1.c, &mb1.i, &mb1.s);
}
