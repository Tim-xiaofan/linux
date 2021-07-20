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
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>

#define BUF_SIZE 1024
#define PORT 10000

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

int main(int ac, char * av[])
{
	struct sockaddr_in localaddr, remoteaddr;
	int sockfd, ret, yes;
	socklen_t addrlen;
	char buf[BUF_SIZE];
	time_t tm;

	if(ac < 2)
	{
		printf("Usage : ./program mode (broadcast-addr)\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, signal_handle);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);//面向消息的UDP
	if(socket < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	if(strncmp("recv", av[1], 4) == 0)
	{
		printf("mode is recv\n");

		sockfd = socket(AF_INET, SOCK_DGRAM, 0);//面向消息的UDP
		if(ac != 2)
		{
			printf("Usage : ./program recv\n");
			exit(EXIT_FAILURE);
		}
		localaddr.sin_family = AF_INET;
		localaddr.sin_addr.s_addr = INADDR_ANY;//必须指定接收的IP数据为任意IP
		localaddr.sin_port = htons(PORT);
		ret = bind(sockfd, (struct sockaddr *)&localaddr, sizeof(struct sockaddr));
		if(ret < 0)
		{
			perror("bind");
			exit(errno);
		}
		printf("bind(%s:%d)\n", 
					inet_ntoa(localaddr.sin_addr), 
					ntohs(localaddr.sin_port));
		while(!force_quit)
		{
			ret = recvfrom(sockfd, 
						buf,
						BUF_SIZE - 1,
						0,
						(struct sockaddr *) &remoteaddr, &addrlen);
			if(ret <= 0)
			{
				perror("recvfrom ");
				break;
			}
			else
			{
				printf("recvfrom (%s:%d), len=%d : %s\n", 
							inet_ntoa(remoteaddr.sin_addr), 
							ntohs(remoteaddr.sin_port), ret, buf);
			}
		}
	}
	else if(strncmp("send", av[1], 4) == 0)
	{
		printf("mode is send\n");
		if(ac != 3)
		{
			printf("Usage : ./program send broadcast-addr\n");
			exit(EXIT_FAILURE);
		}

		yes = 1;
		ret = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));
		if(ret < 0)
		{
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}
		remoteaddr.sin_family = AF_INET;
		remoteaddr.sin_addr.s_addr = inet_addr(av[2]);
		remoteaddr.sin_port = htons(PORT);
		addrlen = sizeof(struct sockaddr_in);
		localaddr.sin_family = AF_INET;
		localaddr.sin_port = htons(PORT);

		ret = bind(sockfd, (const struct sockaddr *)&localaddr, sizeof(struct sockaddr));
		if(ret < 0)
		{
			perror("bind2");
			exit(errno);
		}
		printf("bind(%s:%d)", 
					inet_ntoa(localaddr.sin_addr), 
					ntohs(localaddr.sin_port));

		while(!force_quit)
		{
			bzero(buf, BUF_SIZE);
			tm = time(NULL);
			sprintf(buf, "%s", ctime(&tm));//broadcast time
			ret = sendto(sockfd, 
						buf,
						strlen(buf) + 1,
						0,
						(const struct sockaddr *) &remoteaddr,
						addrlen);
			if(ret <= 0)
			{
				perror("sendto");
				break;
			}
			else
			{
				printf("sendto (%s:%d), len=%d : %s\n", 
							inet_ntoa(remoteaddr.sin_addr), 
							ntohs(remoteaddr.sin_port), ret, buf);
			}
			usleep(100);
		}
	}
	else
	{
		fprintf(stderr, "unkown mode %s\n", av[1]);
	}
	close(sockfd);

	return 0;
}
