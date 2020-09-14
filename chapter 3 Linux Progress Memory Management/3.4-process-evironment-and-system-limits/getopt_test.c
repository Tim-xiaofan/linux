#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	//printf("hello getopt\n");
	/*单个字符a选项
	 * 单个字符b:后面必须跟一个参数，可以紧跟或者空格
	 * c::可以紧跟一个参数
	 * 参数都放在全局变量optarg中*/
	const char* optstring = "ab:c::";
	int result = 0;
	opterr = 0;//不向stderr输出参数
	for(result = 1; result < argc; result++)
                printf("at start argv[%d] = %s\n", result, argv[result]);
	while((result = getopt(argc, argv, optstring)) != -1)
	{	
		/*一直解析
		 * 信息存储在全局变量中，如下
		 * extern char *optarg 选项的赋值，-b hello 则optarg 指向hello 
		 * extern int optint 下一个参数的下标
		 * extern int opterr 为0时不向stderr输出错误信息
		 * extern int optopt 当选项不在optstring中时或缺少必要参数时，存储其中。getopt返回？*/
		switch(result)
		{
			case 'a': printf("option = a, optopt = %c, optarg = %s\n", optopt, optarg);
				 break;
			case 'b': printf("option = b, optopt = %c, optarg = %s\n", optopt, optarg);
                                 break;
			case 'c': printf("option = c, optopt = %c, optarg = %s\n", optopt, optarg);
                                 break;
			case 'd': printf("option = d, optopt = %c, optarg = %s\n", optopt, optarg);
                                 break;
			case '?': printf("option = ?, optopt = %c, optarg = %s\n", optopt, optarg);
                                 break;
			default: printf("default, result = %c\n", result);
				 break;
		}
		printf("argv[%d] = %s\n", optind, argv[optind]);
	}
	printf("result = -1, optind = %d\n", optind);//打印最后可能出错的位置
	for(result = optind; result < argc; result++)
		printf("---argv[%d] = %s\n", result, argv[result]);//打印余下的错误选项
	for(result = 1; result < argc; result++)
		printf("at end argv[%d] = %s\n", result, argv[result]);	
}
