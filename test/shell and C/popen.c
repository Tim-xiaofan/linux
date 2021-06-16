/** 20210323 11:36, zyj ,GuangDong*/
//创建一个连接到另一个进程（shell的命令行）的管道，然后读其输出或向其输入端发送数据
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

int main()
{
	FILE *fp = NULL, *fp1 = NULL;
	char data[1024] = {'0'};
	char fmt[2048] = {'0'};
	int len = 0;

	fp = popen("ls *.h", "r");
	if (fp == NULL)
	{
		printf("popen error : %s\n", strerror(errno));
		return 1;
	}
	fp1 = fopen("./s1ap.h", "w");
	if (fp1 == NULL)
	{
		printf("s1ap error : %s\n", strerror(errno));
		return 1;
	}

	while (fgets(data, sizeof(data), fp) != NULL)
	{
		len = strlen(data);
		data[len -1] = '\0';
		sprintf(fmt, "#include \"%s\"\n", data);
		printf("%s", fmt);
		fpintf(fp1, "%s", fmt);
	}
	pclose(fp);
	fclose(fp1);
	return 0;
}
