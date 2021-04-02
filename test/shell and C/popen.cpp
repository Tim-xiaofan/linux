/** 20210323 11:36, zyj ,GuangDong*/
//创建一个连接到另一个进程（shell的命令行）的管道，然后读其输出或向其输入端发送数据
#include<cstdio>
#include<cstdlib>
#include<ctime>
int main()
{
	using namespace std;
	FILE *fp = NULL;
	char data[100] = {'0'};
	int ct = 0;
	time_t tt;

	fp = popen("cat ~/.bash_history", "r");
	if (fp == NULL)
	{
		printf("popen error!\n");
		return 1;
	}
//	tt = time(NULL);
//	printf("tt = %lu\n", tt);

	while (fgets(data, sizeof(data), fp) != NULL && (++ct) <= 10)
	{
		tt = atol(&data[1]);
		//printf("%lu\n", tt);
		if(tt != 0)
		  printf("%s\n", ctime(&tt));
		printf("%s", data);
	}
	pclose(fp);
	return 0;
}
