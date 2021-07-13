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
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

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
	fd_set rdfds, backfds;//申明一个fd_set集合来保存要检测的文件描述符
	int err;
	
	signal(SIGINT, signal_handle);

	/** tcp-server*/
	if(ac != 3)
	{
		fprintf(stderr, "Invalid argument.\n");
		printf("Usage : ./program ip port\n");
		exit(EXIT_FAILURE);
	}
	int ssock, csock = -1, ret, addr_len, max_fd, nread, echo_ct = 0;
	struct sockaddr_in local_addr, remote_addr;
	local_addr.sin_addr.s_addr = inet_addr(av[1]);
	local_addr.sin_port = htons(atoi(av[2]));
	local_addr.sin_family = AF_INET;
	addr_len = sizeof(struct sockaddr_in);

	ssock = socket(PF_INET, SOCK_STREAM, 0);
	if(ssock < 0)
	{
		fprintf(stderr, "socket : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	ret = bind(ssock, (const struct sockaddr *)&local_addr, sizeof(struct sockaddr));
	if(ret < 0)
	{
		fprintf(stderr, "bind : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	ret = listen(ssock, 5);
	if(ret < 0)
	{
		fprintf(stderr, "listen : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	FD_ZERO(&rdfds);//用select函数之前先把集合清零
	FD_ZERO(&backfds);
	FD_SET(ssock, &backfds);
	max_fd = ssock;

	while(!force_quit)
	{
		/*无限期阻塞，并测试文件描述符变动 */
		printf("Server is waiting(max_fd = %d)...\n", max_fd);
		rdfds = backfds;//将需要监视的描述符集copy到select查询队列中，select会对其修改，所以一定要分开使用变量
		err = select(max_fd + 1, &rdfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0); //FD_SETSIZE：系统默认的最大文件描述符
		if(err == 0) //超时
		{
			printf("select time out!\n");
			break;
		}
		else if(err == -1)  //失败
		{
			printf("select : %s\n", strerror(errno));
			break;
		}
		else  //成功
		{
			//printf("max_fd = %d\n", max_fd);
			for(int fd = 0; fd <= max_fd; ++fd)
			{
				if(FD_ISSET(fd, &rdfds))
				{
					printf("Event on fd %d\n", fd);
					if(fd == ssock)// server
					{
						csock = accept(ssock, (struct sockaddr *)&remote_addr, (socklen_t *)&addr_len);
						if(csock < 0)
						{
							fprintf(stderr, "accept : %s\n", strerror(errno));
							exit(EXIT_FAILURE);
						}
						printf("Got a connection from %s : %d\n", 
									inet_ntoa(remote_addr.sin_addr), 
									ntohs(remote_addr.sin_port));
						FD_SET(csock, &backfds);
						max_fd = (csock > max_fd) ? csock : max_fd;
						//exit(EXIT_FAILURE);
					}
					else//client
					{
						ioctl(fd, FIONREAD, &nread); //取得数据量交给nread
						/*客户数据请求完毕，关闭套接字，从集合中清除相应描述符 */
						if (nread == 0)
						{
							close(fd);
							FD_CLR(fd, &backfds); //去掉关闭的fd
							printf("removing client on fd %d\n", fd);
						}
						else /*处理客户数据请求*/
						{
							char buf[1024];
							memset(buf, '\0', 1024);
							read(fd, buf, 1024);
							printf("Got data from client on fd %d : %s\n", fd, buf);
							memset(buf, '\0', 1024);
							sprintf(buf, "echo -----> %d\n", echo_ct++);
							write(fd, buf, strlen(buf) + 1);
						}
					}
				}
			}//traverse fds
		}//select ret
	}
	close(ssock);
	for(int fd; fd <= max_fd; ++fd)
	{
		close(fd);
	}
	printf("Bye.\n");
	return 0;
}
