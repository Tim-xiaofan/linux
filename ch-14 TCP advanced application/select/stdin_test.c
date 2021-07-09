/* stdin_test.c */
#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <errno.h>

static bool force_quit = false;

/* On all other architectures */
//int pipe(int pipefd[2]);
void signal_handle(int signum)
{
	if(signum == SIGINT)
	{
		printf("Preparing to quit...\n");
		force_quit = true;
	}
}

int main()
{
	fd_set rdfds;//申明一个fd_set集合来保存要检测的文件描述符
	struct timeval tv;
	int err, pipefd[2];
	pid_t pid;
	char buf1[1024], buf2[1024];
	time_t tm;

	FD_ZERO(&rdfds);//用select函数之前先把集合清零
	int fd = 0;//标准输入输出
	tv.tv_sec = 30; /* 超时事件设置为5s */
	tv.tv_usec = 500;//设置select等待的最大时间为3s加500ms
	srand(time(NULL));
	signal(SIGINT, signal_handle);

	/**pipe op */
	err = pipe(pipefd);
	if(err != 0)
	{
		perror("pipe : ");
		exit(EXIT_FAILURE);
	}

	pid = fork();
	if(pid == 0)//child to write
	{
		printf("Enter child process\n");
		close(pipefd[0]);//close read endpoint
		while(!force_quit)
		{
			tm = time(NULL);
			sprintf(buf1, "%s", ctime(&tm));
			write(pipefd[1], buf1, strlen(buf1) + 1);//write to fd[1]
			printf("child write : %s\n", buf1);
			sleep(rand() % 10);
		}
		printf("Leave child process\n");
	}
	else if(pid > 0)//parent to read
	{
		printf("Enter parent process\n");
		close(pipefd[1]);//close write endpoint
		printf("pipefd[0] = %d, fd = %d\n", pipefd[0], fd);
		while(!force_quit)
		{
			/*每次调用select之前都要重新在read_fds和exception_fds中设置文件描述符connfd，因为事件发生以后，文件描述符集合将被内核修改*/
			FD_SET(fd, &rdfds);/* 对标准输入描述符感兴趣(把要检测的文件描述符socket加入到集合里) */
			FD_SET(pipefd[0], &rdfds);//pipefd read endpoint
			err = select(pipefd[0] + 1, &rdfds, NULL, NULL, &tv);

			if(err == 0) //超时
			{
				//printf("select time out!\n");
				continue;
			}
			else if(err == -1)  //失败
			{
				printf("fail to select : %s\n", strerror(errno));
				break;
			}
			else  //成功
			{
				if(FD_ISSET(fd, &rdfds))
				{
					char buf[128];
					read(fd, buf, 128);
					printf("Got data from stdin : %s\n", buf);
				}
				if(FD_ISSET(pipefd[0], &rdfds))
				{
					read(pipefd[0], buf2, 1024);//read from fd[0]
					printf("Got data from pipefd : %s\n", buf2);
				}
			}

		}
		printf("Leave parent process\n");
	}
	return 0;
}
