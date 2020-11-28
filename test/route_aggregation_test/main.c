/*main.c*/
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	char a[]="192.24.0.0";
	char b[]="192.24.16.0";
	char c[]="192.24.8.0";

	/*IPv4 numbers-and-dots notation to binary data in network byte  order*/
	in_addr_t net_a, net_b, net_c;
	net_a = inet_addr(a);
	net_b = inet_addr(b);
	net_c = inet_addr(c);

	/*rout aggregation*/
	in_addr_t rout_aggr;
	rout_aggr = net_a & net_b  & net_c;

	/*converts the Internet host address in, given in network byte order, to a string in IPv4  dotted-decimal  notation*/
	struct in_addr in;
	in.s_addr = rout_aggr;
	char *addr = inet_ntoa(in);

	/*output*/
	printf("%s\n", addr);
}
