#include <sys/types.h>
#include <sys/socket.h> 
#include <stdio.h>
#include <string.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#define BUFSIZE 128

int main(int ac, char *av[])
{
	int server_sockfd, client_sockfd;
	struct sockaddr_in server_address, client_address;
	int i, byte, client_len;
	char char_send[BUFSIZE];

	printf("%d\n", MSG_DONTWAIT);
	if(ac != 3)
	{
		fprintf(stderr, "not enough cmdline args\n");
		printf("./tcp_unblock_server ip port\n");
		exit(EXIT_FAILURE);
	}

	/** set address family and (ip, port)*/
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(av[1]);
	server_address.sin_port = htons(atoi(av[2]));

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);//创建 socket 对象

	/** bind and listen*/
	if(bind(server_sockfd, (const struct sockaddr *)&server_address, sizeof(server_address)) != 0)
	{
		fprintf(stderr, "bind error : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	if(listen(server_sockfd, 5) != 0)
	{
		fprintf(stderr, "listen error : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("server waiting for connect on %s:%u\n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));
	client_len = sizeof(client_address);	
	client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t *)&client_len);
	printf("got a connection from ip=%s, port=%u\n", 
				inet_ntoa(client_address.sin_addr), 
				ntohs(client_address.sin_port));

	for(i = 0; i < 5; ++i)
	{
		memset(char_send, '\0', BUFSIZE);
		printf("input message to send: ");
		//首先阻塞在终端，接收用户输入数据 
		fgets(char_send, BUFSIZE, stdin);
		if((byte=send(client_sockfd,char_send,strlen(char_send),0)) == -1)
		{
			perror("send : ");
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("sent msg : %s\n", char_send);
		}

		memset(char_send, '\0', BUFSIZE);
		byte= recv(client_sockfd, char_send, BUFSIZE, MSG_DONTWAIT);//非阻塞接收
		if(byte > 0)
		{
			printf("got msg : %s\n", char_send);
		}
		else if(byte < 0)
		{
			if(errno == EAGAIN)
			{
				printf("recv returned no data\n");
			}
			else
			{
				perror("recv : ");
				exit(EXIT_FAILURE);
			}
		}
	}//for 5

	shutdown(server_sockfd, 2);
	shutdown(client_sockfd, 2);
	return 0;
}
