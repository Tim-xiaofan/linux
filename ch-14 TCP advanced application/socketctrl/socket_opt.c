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
	int sock_fd, ret, addr_len, rcvbuf_size, sndbuf_size, type;
	socklen_t size;
	struct sockaddr_in local_addr;
	struct timeval ret_time, set_time;

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
	ret = getsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &sndbuf_size, &size);//获取发送缓冲区大小
	if(ret == -1)
	{
		perror("getsockopt ");
		exit(errno);
	}
	printf("sndbuf_size=%d, size = %d\n",sndbuf_size, size);

	size=sizeof(sndbuf_size);
	ret = getsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf_size, &size);//获取发送缓冲区大小
	if(ret == -1)
	{
		perror("getsockopt ");
		exit(errno);
	}
	printf("rcvbuf_size=%d, size = %d\n",rcvbuf_size, size);

	size=sizeof (type);//socket类型 
	ret = getsockopt(sock_fd, SOL_SOCKET, SO_TYPE, &type, &size);
	if(ret == -1)
	{
		perror("getsockopt ");
		exit(errno);
	}
	printf("socket type=%d\n", type);

	size=sizeof(struct timeval);
	ret = getsockopt(sock_fd, SOL_SOCKET, SO_SNDTIMEO, &ret_time, &size);//发送超时值
	if(ret == -1)
	{
		perror("getsockopt ");
		exit(errno);
	}
	printf("default : time out is %lds, and %ldms\n", ret_time.tv_sec, ret_time.tv_usec);
	set_time.tv_sec=10;
	set_time.tv_usec=4000 + 3999;
	ret = setsockopt(sock_fd, SOL_SOCKET, SO_SNDTIMEO, &set_time, size);//修改该值
	if(ret == -1)
	{
		perror("setsockopt ");
		exit(errno);
	}
	ret = getsockopt(sock_fd, SOL_SOCKET, SO_SNDTIMEO, &ret_time, &size);
	if(ret == -1)
	{
		perror("getsockopt ");
		exit(errno);
	}
	printf("after modifying : time out is %lds, and %ldms\n", ret_time.tv_sec, ret_time.tv_usec);

	int ttl = 0;
	size=sizeof(ttl); //读取 TTL 值
	ret = getsockopt(sock_fd, IPPROTO_IP, IP_TTL, &ttl, &size);
	if(ret == -1)
	{
		perror("getsockopt ");
		exit(errno);
	}
	printf("the default ip ttl is %d\n",ttl);

	int maxseg=0;
	size=sizeof (maxseg); //读取 TCP_MAXSEG 值 
	getsockopt(sock_fd, IPPROTO_TCP, TCP_MAXSEG, &maxseg,&size);
	printf("the TCP max seg is %d\n",maxseg);

	close(sock_fd);
	return 0;
}
