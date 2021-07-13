#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>

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

int main(int ac, char * av[])
{
	int ssock, csock, ret, addr_len, stdin_fd = 0;
	struct sockaddr_in local_addr, remote_addr;
	struct pollfd fds[2];
	char buf[BUF_SIZE];

	signal(SIGINT, signal_handle);
	
	ssock = socket(PF_INET, SOCK_STREAM, 0);
	if(ssock < 0)
	{
		perror("socket ");
		exit(EXIT_FAILURE);
	}
			
	if(ac != 3)
	{
		fprintf(stderr, "Invalid cmdline args\n");
		printf("Usage : ./program local_addr local_port\n");
		exit(EXIT_FAILURE);
	}
	local_addr.sin_addr.s_addr = inet_addr(av[1]);
	local_addr.sin_port = htons(atoi(av[2]));
	local_addr.sin_family = AF_INET;
	addr_len = sizeof(struct sockaddr_in);
	ret = bind(ssock, (const struct sockaddr *)&local_addr, addr_len);
	if(ret < 0)
	{
		perror("bind ");
		exit(EXIT_FAILURE);
	}

	ret = listen(ssock, 5);
	if(ret < 0)
	{
		perror("listen ");
		exit(EXIT_FAILURE);
	}

	printf("Waiting for connection...");
	csock = accept(ssock, (struct sockaddr *) & remote_addr, (socklen_t *) & addr_len);
	if(csock < 0)
	{
		perror("accept ");
		exit(EXIT_FAILURE);
	}

	printf("Got a connection from (%s : %d)\n", inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port));
	fds[0].fd = stdin_fd;
	fds[0].events = POLLIN;//data to read
	fds[1].fd = csock;
	fds[1].events = POLLIN;

	while(!force_quit)
	{
		ret = poll(fds, 2, -1);// no timeout
		if(ret > 0)
		{
			if((fds[0].revents & POLLIN ) ==  POLLIN )
			{ // 标准输入
				memset(buf, 0, BUF_SIZE);
				read(fds[0].fd, buf, BUF_SIZE);
				printf("stdin : %s\n", buf);
            }
			if(( fds[1].revents & POLLIN ) ==  POLLIN )
			{ //udp套接字
				memset(buf, 0, BUF_SIZE);
				read(fds[1].fd, buf, BUF_SIZE);
				printf("tcp : %s\n", buf);
            }
		}
		else
		{
			perror("poll : ");
			break;
		}
	}
	close(csock);
	close(ssock);
	return 0;
}
