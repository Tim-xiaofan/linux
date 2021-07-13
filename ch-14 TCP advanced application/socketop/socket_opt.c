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
	int sock_fd, ret, addr_len, rcvbuf_size, sndbuf_size;
	socklen_t size;
	struct sockaddr_in local_addr;

	signal(SIGINT, signal_handle);

	sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0)
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
	ret = bind(sock_fd, (const struct sockaddr *)&local_addr, addr_len);
	if(ret < 0)
	{
		perror("bind ");
		exit(EXIT_FAILURE);
	}

	ret = listen(sock_fd, 5);
	if(ret < 0)
	{
		perror("listen ");
		exit(EXIT_FAILURE);
	}

	size=sizeof(sndbuf_size);
	getsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &sndbuf_size, &size);//获取发送缓冲区大小
	printf("sndbuf_size=%d, size = %d\n",sndbuf_size, size);

	size=sizeof(sndbuf_size);
	getsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf_size, &size);//获取发送缓冲区大小
	printf("rcvbuf_size=%d, size = %d\n",rcvbuf_size, size);

	close(sock_fd);
	return 0;
}
