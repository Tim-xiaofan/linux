/** 20210323 11:27, zyj ,GuangDong*/
#include <stdio.h>
#include <stdlib.h>
int main()
{
	int ret = 0;
	ret = system(NULL);
	printf("\"NULL\" : ret = %d \n",ret);//ret = 1

	ret = system("exit 55");
	printf("\"exit 55\": ret = %d \n",ret);//ret = 14080

	ret = system("ls dsfgsdfg");//ls, mo such file  ret = 512
	printf("\"ls dsfgsdfg\" : ret = %d \n",ret);

	ret = system("asdfgdsgs");//no such cmd, ret = 32512
	printf("\"asdfgdsgs\" : ret = %d \n",ret);

	ret = system("ls");// normal
	printf("\"ls\" : ret = %d \n",ret);
	return 0;
}
