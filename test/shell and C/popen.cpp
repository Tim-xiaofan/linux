/** 20210323 11:36, zyj ,GuangDong*/
//创建一个连接到另一个进程（shell的命令行）的管道，然后读其输出或向其输入端发送数据
#include<stdio.h>
int main()
{
	FILE *fp = NULL;
	char data[100] = {'0'};
	int ct = 0;

	fp = popen("last", "r");
	if (fp == NULL)
	{
		printf("popen error!\n");
		return 1;
	}

	while (fgets(data, sizeof(data), fp) != NULL)
	{
		printf("%03d\t%s", ++ct, data);
	}
	pclose(fp);
	return 0;
}
