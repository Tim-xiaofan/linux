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
#include <time.h>
#include <sys/wait.h>

#define CLIENT_MAX_NB 512
#define BUF_MAX_SIZE 2048

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
	int err, csock, ret, addr_len, max_fd = 0, nread, i, max_client_ct;
	struct sockaddr_in local_addr, remote_addr;
	char rbuf[BUF_MAX_SIZE], ipstr1[32], ipstr2[32], sbuf[BUF_MAX_SIZE];
	pid_t pid;
	time_t tm;
	ssize_t rbytes;

	signal(SIGINT, signal_handle);
	srand(time(NULL));

	/** tcp-server*/
	if(ac != 5)
	{
		fprintf(stderr, "Invalid argument.\n");
		printf("Usage : ./program src_ip dst_ip dst_port max_client_ct\n");
		exit(EXIT_FAILURE);
	}
	remote_addr.sin_addr.s_addr = inet_addr(av[2]);
	remote_addr.sin_port = htons(atoi(av[3]));
	remote_addr.sin_family = AF_INET;
	addr_len = sizeof(struct sockaddr_in);
	local_addr.sin_family = AF_INET;
	max_client_ct = atoi(av[4]);

	FD_ZERO(&backfds);
	for(i = 0; i < max_client_ct; ++i)
	{
		csock = socket(PF_INET, SOCK_STREAM, 0);
		if(csock < 0)
		{
			fprintf(stderr, "socket : %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		max_fd = (csock > max_fd) ? csock : max_fd;
		FD_SET(csock, &backfds);
		ret = bind(csock, (struct sockaddr *)&local_addr, addr_len);
		if(ret < 0)
		{
			fprintf(stderr, "bind : %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		ret = connect(csock, (const struct sockaddr *) & remote_addr, addr_len);
		if(ret < 0)
		{
			fprintf(stderr, "connect : %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		sprintf(ipstr1, "%s", inet_ntoa(local_addr.sin_addr));
		sprintf(ipstr2, "%s", inet_ntoa(remote_addr.sin_addr));
		printf("(%s : %d) connected to (%s : %d)\n",
					ipstr1, ntohs(local_addr.sin_port),
					ipstr2, ntohs(remote_addr.sin_port));
		memset(sbuf, '\0', BUF_MAX_SIZE);
		sprintf(sbuf, "I'm client-%d.\n", i);
		write(csock, sbuf, strlen(sbuf) + 1);
	}

	pid = fork();
	if(pid == 0)
	{
		printf("Enter child process\n");
		int fd;
		while(!force_quit)
		{
			fd = rand() % (max_fd + 1);
			if(FD_ISSET(fd, &backfds))
			{
				memset(sbuf, '\0', BUF_MAX_SIZE);
				tm = time(NULL);
				sprintf(sbuf, "client %03d, time %s\n", fd, ctime(&tm));
				if(write(fd, sbuf, strlen(sbuf) + 1) < 0)
				{
					fprintf(stderr, "write : %s\n", strerror(errno));
					if(errno == EPIPE)
					{
						fprintf(stderr, "Peer has closed, close %d.\n", fd);
						FD_CLR(fd, &backfds);
						close(fd);
					}
					continue;
				}
				printf("send : %s", sbuf);
				usleep(rand() % 21);// 0~20ms
			}
		}
		printf("Leave child process\n");
	}
	else if(pid > 0)
	{
		printf("Leave parent process\n");
		while(!force_quit)
		{
			/*无限期阻塞，并测试文件描述符变动 */
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
						ioctl(fd, FIONREAD, &nread); //取得数据量交给nread
						/*客户数据请求完毕，关闭套接字，从集合中清除相应描述符 */
						if (nread == 0)
						{
							close(fd);
							FD_CLR(fd, &backfds); //去掉关闭的fd
							printf("#1 removing client on fd %d\n", fd);
						}
						else /*处理客户数据请求*/
						{
							memset(rbuf, '\0', 1024);
							rbytes = read(fd, rbuf, 1024);
							if(rbytes == 0)
							{
								FD_CLR(fd, &backfds); //去掉关闭的fd
								printf("#2 removing client on fd %d\n", fd);
							}
							else printf("Got data from client on fd %d : %s\n", fd, rbuf);
						}
					}
				}//traverse fds
			}//select ret
		}
		wait(0);
		printf("Leave parent process\n");
	}
	else
	{
		fprintf(stderr, "fork : %s\n", strerror(errno));
	}

	for(int fd; fd <= max_fd; ++fd)
	{
		close(fd);
	}
	printf("Bye.\n");
	return 0;
}
