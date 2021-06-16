/* stdin_test.c */
#include <stdio.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	fd_set rd;
	struct timeval tv;
	int err;

	FD_ZERO(&rd);
	int fd = 0;
	tv.tv_sec = 5; /* 超时事件设置为5s */
	tv.tv_usec = 0;
	while(1)
	{
		FD_SET(fd, &rd);/* 对标准输入描述符感兴趣 */

		err = select(fd + 1, &rd, NULL, NULL, NULL);

		if(err == 0) //超时
		{
			printf("select time out!\n");
			break;
		}
		else if(err == -1)  //失败
		{
			printf("fail to select!\n");
			break;
		}
		else  //成功
		{
			if(FD_ISSET(fd, &rd))
			{
				char buf[128];
				read(fd, buf, 128);
				printf("data is available and data is %s\n", buf);
			}
		}

	}
	return 0;
}
