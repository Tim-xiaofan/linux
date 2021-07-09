#include <stdio.h> 
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h> 
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#define MAXBUF 128

int main(int ac, char * av[])
{
	int sockfd, ret;
	struct sockaddr_in dest, mine;
	char buffer[MAXBUF + 1];

	if(ac != 4)
	{
		fprintf(stderr, "invalid argument\n");
		printf("./tcp_unblock_client dst_ip dst_port src_ip\n");
		exit(EXIT_FAILURE);
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket : ");
		exit(EXIT_FAILURE);
	}

	/** server's address*/
	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(atoi(av[2]));//server port
	if(inet_aton(av[1], (struct in_addr *)&dest.sin_addr.s_addr) == 0)
	{
		perror(av[1]);
		exit(EXIT_FAILURE);
	}

	/** client address*/
	bzero(&mine, sizeof(mine));
	mine.sin_family = AF_INET;
	mine.sin_port = htons(7839);
	if(inet_aton(av[3], (struct in_addr *)&mine.sin_addr.s_addr) == 0)
	{
		perror(av[3]);
		exit(EXIT_FAILURE);
	}

	//bind
	if(bind(sockfd, (struct sockaddr *) & mine, sizeof(struct sockaddr)) < 0)
	{
		perror("bind :");
		exit(EXIT_FAILURE);
	}
	
	printf("client connects to %s:%u\n", inet_ntoa(dest.sin_addr), ntohs(dest.sin_port));
	if(connect(sockfd, (struct sockaddr *)& dest, sizeof(struct sockaddr)) < 0)
	{
		perror("connect :");
		exit(EXIT_FAILURE);
	}

	/** set to unblock*/
	if(fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("fcntl :");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		bzero(buffer, MAXBUF);
		ret = recv(sockfd, buffer, MAXBUF, 0); //  flow fcntl : nonblock
		if(ret < 0)
		{
			if(errno == EAGAIN)
			{
				errno = 0;
				//fprintf(stderr, "no data\n");
				continue;
			}
			else
			{
				perror("recv");
				exit (EXIT_FAILURE);
			}
		}
		else if(ret > 0)
		{
			printf("recv %s\n", buffer);
			ret = 0;
		}

		//send
		memset(buffer, '\0', MAXBUF + 1);
		printf("input msg to send : ");
		fgets(buffer, MAXBUF, stdin);
		if((ret = send(sockfd, buffer, strlen(buffer), 0)) < 0)
		{
			perror("send : ");
			exit(EXIT_FAILURE);
		}
	}
	close(sockfd);
	return 0;
}
