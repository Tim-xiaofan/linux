#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#include <fcntl.h>
#endif

#define BUF_SIZE 1024

static bool force_quit = false;

void signal_handle(int signum)
{
	if(signum == SIGINT)
	{
		printf("Preparing to quit...\n");
		force_quit = true;
	}
}

static void fcntl_flags(int flags)
{
	if(O_RDONLY & flags) printf("O_RDONLY\n");
	if(O_WRONLY & flags) printf("O_WRONLY\n");
	if(O_RDWR & flags) printf("O_RDWR\n");
	if(O_CREAT & flags) printf("O_CREAT\n");
	if(O_EXCL & flags) printf("O_EXCL\n");
	if(O_NOCTTY & flags) printf("O_NOCTTY\n");
	if(O_TRUNC & flags) printf("O_TRUNC\n");
	if(O_APPEND & flags) printf("O_APPEND\n");
	if(O_ASYNC & flags) printf("O_ASYNC\n");
	//if(O_DIRECT & flags) printf("O_DIRECT\n");
	//if(O_NOATIME & flags) printf("O_NOATIME\n");
	if(O_NONBLOCK & flags) printf("O_NONBLOCK\n");
	if(O_DSYNC & flags) printf("O_DSYNC\n");
	if(O_SYNC & flags) printf("O_SYNC\n");
}

int main(int ac, char * av[])
{
	int fd, flags, pid;

	fd = socket(PF_INET, SOCK_STREAM, 0);
	if(fd < 0)
	{
		perror("socket ");
		exit(EXIT_FAILURE);
	}
	flags = fcntl(fd, F_GETFL, 0);
	if(flags < 0)
	{
		perror("fcntl F_GETFL");
		exit(errno);
	}
	printf("flags = %d\n", flags);
	fcntl_flags(flags);
	
	flags |= O_NONBLOCK;
	flags = fcntl(fd, F_SETFL, flags);
	if(flags < 0)
	{
		perror("fcntl F_SETFL");
		exit(errno);
	}
	flags = fcntl(fd, F_GETFL, 0);
	if(flags < 0)
	{
		perror("fcntl F_GETFL");
		exit(errno);
	}
	printf("after setting flags = %d\n", flags);
	fcntl_flags(flags);

	pid = fcntl(fd, F_GETOWN, getpid());
	if(pid < 0)
	{
		perror("fcntl F_GETOWN");
		exit(errno);
	}
	printf("pid = %d\n", pid);

	close(fd);
	return 0;
}
